#pragma once
#include "nam-core/NAM/dsp.h"
#include "../tonestack/ToneStack.h"
#include "../noise-gate/NoiseGate.h"
#include "../resampler/Resampler.h"
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <string>
#include <vector>
#include <array>

namespace nam_processor {
    class AbstractNAMProcessor {
    public:
        virtual ~AbstractNAMProcessor() = default;

        virtual bool loadModel(const std::string& file) = 0;
        virtual void process(juce::AudioBuffer<float>& buffer) = 0;
        virtual void prepareToPlay(const juce::dsp::ProcessSpec& spec) = 0;
        virtual void hookParameters(const juce::AudioProcessorValueTreeState& apvts) = 0;
        virtual void clearModel() = 0;
        virtual void resetModel() const = 0;
        virtual bool isModelLoaded() const = 0;

    protected:
        double mSampleRate{};
        int mSamplesPerBlock{};
        juce::uint32 mNumChannels{};
        std::string mModelPath;
        std::atomic<bool> filesUpdated{false};
        std::array<std::atomic<float>*, 9> params{};
        bool modelLoaded{false};
        bool shouldRemoveModel{false};
        bool outputNormalized{false};

        virtual void updateParameters() = 0;
        virtual void applyDSPStaging() = 0;
    };

    class NAMProcessor final : public AbstractNAMProcessor {
    public:
        NAMProcessor();
        ~NAMProcessor() override;
        bool loadModel(const std::string& file) override;
        void process(juce::AudioBuffer<float>& buffer) override;

        static void processStage(float bypassParam, const std::function<void()>& stageProcessor);
        void prepareToPlay(const juce::dsp::ProcessSpec& spec) override;

        static void dualMono(juce::AudioBuffer<float>& mainBuffer, float** input);
        void hookParameters(const juce::AudioProcessorValueTreeState& apvts) override;
        static void createParameters(std::vector<std::unique_ptr<juce::RangedAudioParameter>>& parameters);
        void clearModel() override;
        void applyDSPStaging() override;
        void resetModel() const override;
        bool isModelLoaded() const override;

        enum Parameters {
            kInput,
            kOutput,
            kToneBass,
            kToneMid,
            kToneTreble,
            kToneStackBypass,
            kNoiseGateThreshold,
            kNoiseGateBypass,
            kNormalizeOutput
        };

    private:
        juce::AudioBuffer<float> outputBuffer;
        std::unique_ptr<dsp::tone_stack::AbstractToneStack> mToneStack;
        std::unique_ptr<dsp::noise_gate::AbstractNoiseGateProcessor> mNoiseGate;
        std::unique_ptr<ResamplingNAM> mModel, mStagedModel;
        void updateParameters() override;
    };
}; // namespace nam_processor