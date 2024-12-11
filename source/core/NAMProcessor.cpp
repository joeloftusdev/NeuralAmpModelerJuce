#include "NAMProcessor.h"

#ifdef NAM_SAMPLE_FLOAT
#else
typedef double NAM_SAMPLE;
#endif

namespace nam_processor {
    NAMProcessor::NAMProcessor()
    {
        mToneStack = std::make_unique<dsp::tone_stack::BasicNamToneStack>();
        mNoiseGate = std::make_unique<dsp::noise_gate::NoiseGateProcessor>();
        nam::activations::Activation::enable_fast_tanh();
    }

    NAMProcessor::~NAMProcessor()
    = default;

    void NAMProcessor::prepareToPlay(const juce::dsp::ProcessSpec& spec)
    {
        this->mSampleRate = spec.sampleRate;
        this->mSamplesPerBlock = static_cast<int>(spec.maximumBlockSize);
        this->mNumChannels = spec.numChannels;

        outputBuffer.setSize(1, static_cast<int>(spec.maximumBlockSize), false, false, false);
        outputBuffer.clear();

        mToneStack->prepare(spec);
        mNoiseGate->prepare(spec);

        resetModel();
    }

    bool NAMProcessor::loadModel(const std::string& file)
    {
        try
        {
            const auto dspPath = std::filesystem::u8path(file);
            std::unique_ptr<nam::DSP> model = nam::get_dsp(dspPath);
            if (!model)
            {
                throw std::runtime_error("Failed to create DSP model.");
            }

            auto temp = std::make_unique<ResamplingNAM>(std::move(model), this->mSampleRate);
            if (!temp)
            {
                throw std::runtime_error("Failed to create ResamplingNAM.");
            }
            temp->Reset(this->mSampleRate, this->mSamplesPerBlock);
            mStagedModel = std::move(temp);
            return true;
        }
        catch (const std::runtime_error& e)
        {
            mStagedModel = nullptr;
            std::cerr << "Failed to read DSP module from file: " << file << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }
    }
    void NAMProcessor::processStage(const float bypassParam, const std::function<void()>& stageProcessor)
    {
        if (bypassParam == 1.0f) {
            stageProcessor();
        }
    }

    void NAMProcessor::process(juce::AudioBuffer<float>& buffer) {

        this->applyDSPStaging();
        this->updateParameters();

        auto* inputPointer = buffer.getWritePointer(0);
        auto* outputPointer = outputBuffer.getWritePointer(0);

        const float noiseGateBypass = params[kNoiseGateBypass]->load();
        const float toneStackBypass = params[kToneStackBypass]->load();
        const float inputGain = juce::Decibels::decibelsToGain(params[kInput]->load());
        const float outputGain = juce::Decibels::decibelsToGain(params[kOutput]->load());

        processStage(noiseGateBypass, [&]() { mNoiseGate->process(buffer); });

        if (mModel != nullptr) {
            buffer.applyGain(inputGain);
            mModel->process(inputPointer, outputPointer, buffer.getNumSamples());
            mModel->finalize_(buffer.getNumSamples());

            if (this->outputNormalized && mModel->HasLoudness()) {
                const double loudness = mModel->GetLoudness();
                constexpr double targetLoudness = -18.0;
                const float gain = juce::Decibels::decibelsToGain(static_cast<float>(targetLoudness - loudness));
                outputBuffer.applyGain(gain);
            }
        }

        processStage(toneStackBypass, [&]() { mToneStack->process(outputBuffer); });

        dualMono(buffer, &(mModel != nullptr ? outputPointer : inputPointer));
        buffer.applyGain(outputGain);
    }


    void NAMProcessor::dualMono(juce::AudioBuffer<float>& mainBuffer, float** input) {

        const Eigen::Map<Eigen::ArrayXf> inputArray(input[0], mainBuffer.getNumSamples());
        // NOLINTBEGIN
        Eigen::Map<Eigen::ArrayXf> leftChannel(mainBuffer.getWritePointer(0), mainBuffer.getNumSamples());
        Eigen::Map<Eigen::ArrayXf> rightChannel(mainBuffer.getWritePointer(1), mainBuffer.getNumSamples());
        leftChannel = inputArray;
        rightChannel = inputArray;
        // NOLINTEND
    }

    void NAMProcessor::createParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& parameters)
    {
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT_ID", "INPUT", -20.0f, 20.0f, 0.5f));
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUT_ID", "OUTPUT", -40.0f, 40.0f, 0.5f));
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BASS_ID", "BASS", 0.0f, 10.0f, 5.0f));
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("MIDDLE_ID", "MIDDLE", 0.0f, 10.0f, 5.0f));
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("TREBLE_ID", "TREBLE", 0.0f, 10.0f, 5.0f));
        parameters.push_back(std::make_unique<juce::AudioParameterFloat>("NOISE_GATE_THRESHOLD_ID", "NOISE GATE THRESHOLD", -40.0f, 0.0f, -20.0f));
        parameters.push_back(std::make_unique<juce::AudioParameterBool>("NOISE_GATE_BYPASS_ID", "NOISE GATE BYPASS", false));
        parameters.push_back(std::make_unique<juce::AudioParameterBool>("TONE_STACK_BYPASS_ID", "TONE STACK BYPASS", false));
        parameters.push_back(std::make_unique<juce::AudioParameterBool>("NORMALIZE_OUTPUT_ID", "NORMALIZE OUTPUT", false));

    }

    void NAMProcessor::hookParameters(const juce::AudioProcessorValueTreeState& apvts)
    {
        params[kInput] = apvts.getRawParameterValue("INPUT_ID");
        params[kOutput] = apvts.getRawParameterValue("OUTPUT_ID");
        params[kToneBass] = apvts.getRawParameterValue("BASS_ID");
        params[kToneMid] = apvts.getRawParameterValue("MIDDLE_ID");
        params[kToneTreble] = apvts.getRawParameterValue("TREBLE_ID");
        params[kNoiseGateThreshold] = apvts.getRawParameterValue("NOISE_GATE_THRESHOLD_ID");
        params[kNoiseGateBypass] = apvts.getRawParameterValue("NOISE_GATE_BYPASS_ID");
        params[kToneStackBypass] = apvts.getRawParameterValue("TONE_STACK_BYPASS_ID");
        params[kNormalizeOutput] = apvts.getRawParameterValue("NORMALIZE_OUTPUT_ID");
    }

    void NAMProcessor::updateParameters() {

        outputNormalized = static_cast<bool>(params[kNormalizeOutput]->load());

        if (mToneStack != nullptr)
        {
            mToneStack->setParam("bass", params[kToneBass]->load());
            mToneStack->setParam("middle", params[kToneMid]->load());
            mToneStack->setParam("treble", params[kToneTreble]->load());
        }

        if (mNoiseGate != nullptr)
        {
            mNoiseGate->setThreshold(params[kNoiseGateThreshold]->load());
            mNoiseGate->setRatio(2.0f);
            mNoiseGate->setAttack(5.0f);
            mNoiseGate->setRelease(50.0f);
        }
    }

    bool NAMProcessor::isModelLoaded() const
    {
        return this->modelLoaded;
    }

    void NAMProcessor::clearModel() {
        this->shouldRemoveModel = true;
    }

    void NAMProcessor::applyDSPStaging()
    {
        if (shouldRemoveModel)
        {
            mModel = nullptr;
            shouldRemoveModel = false;
        }

        if (mStagedModel != nullptr)
        {
            mModel = std::move(mStagedModel);
            mStagedModel = nullptr;
            modelLoaded = true;
        }
    }

    void NAMProcessor::resetModel() const {
        if (mStagedModel != nullptr)
            mStagedModel->Reset(this->mSampleRate, this->mSamplesPerBlock);

        else if (mModel != nullptr)
            mModel->Reset(this->mSampleRate, this->mSamplesPerBlock);
    }

} // namespace nam_processor