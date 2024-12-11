
#include "PluginProcessor.h"
#include "PluginEditor.h"

Processor::Processor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) , apvts(*this, nullptr, "Params", createParameters()),
        namProcessor(std::make_unique<nam_processor::NAMProcessor>()),
        irProcessor(std::make_unique<dsp::impulse_response::ImpulseResponse>()),
        stateManager(std::make_unique<state_manager::StateManager>(*this, apvts)){apvts.addParameterListener("IR_BYPASS", this);}
#endif


Processor::~Processor() {irProcessor->clear();}

juce::AudioProcessorValueTreeState::ParameterLayout Processor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    nam_processor::NAMProcessor::createParameters(parameters);
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("IR_BYPASS", "IR Bypass", false));
    return {parameters.begin(), parameters.end()};
}

void Processor::loadNAMModel(const juce::File &filePath)
{
    const std::string model_path = filePath.getFullPathName().toStdString();
    namModelLoaded = namProcessor->loadModel(model_path);
}

void Processor::clearNAM()
{
    namProcessor->clearModel();
    namModelLoaded = false;
}

void Processor::clearImpulseResponse()
{
    irProcessor->clear();
    irLoaded = false;
}

void Processor::loadImpulseResponse(const juce::File& file){
    irProcessor->setImpulseResponse(file);
    irLoaded = true;
}

void Processor::prepareToPlay(const double sampleRate, const int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    this->mSampleRate = sampleRate;
    this->mSamplesPerBlock = samplesPerBlock;

    namProcessor->prepareToPlay(spec);
    namProcessor->hookParameters(apvts);
    namProcessor->clearModel();
    namModelLoaded = false;

    irProcessor->prepare(spec);

    if (storedState.isValid())
    {
        restoreNAMModel(storedState);
        storedState = juce::ValueTree();
    }

    rmsOutput.reset(sampleRate, 0.5);
    rmsOutput.setCurrentAndTargetValue(0.0f);

    rmsInput.reset(sampleRate, 0.5);
    rmsInput.setCurrentAndTargetValue(0.0f);
}

void Processor::releaseResources(){}

void Processor::parameterChanged(const juce::String &parameterID, const float newValue) {
    if (parameterID == "IR_BYPASS")
    {
        irBypass = (newValue > 0.5f);
    }
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool Processor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif


    return true;
#endif
}
#endif


void Processor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;

    const float rmsInputLevel = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    rmsInput.setCurrentAndTargetValue(rmsInputLevel);

    namProcessor->process(buffer);

    if (!irBypass && irLoaded)
    {
        irProcessor->process(buffer);
    }

    const float rmsLevel = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    rmsOutput.setCurrentAndTargetValue(rmsLevel);
}

void Processor::getStateInformation(juce::MemoryBlock& destData) {stateManager->getStateInformation(destData);}

void Processor::setStateInformation(const void* data, const int sizeInBytes)
{
    stateManager->setStateInformation(data, sizeInBytes);
    const auto state = stateManager->getAPVTS().copyState();
    if (const std::unique_ptr<juce::XmlElement> xml = state.createXml(); xml == nullptr)
    {
        throw std::runtime_error("Failed to create XML from state.");
    }

    restoreImpulseResponse(state);
    storedState = state;
}

void Processor::restoreImpulseResponse(const juce::ValueTree& state)
{
    const auto impulseResponseFilePath = state.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    if (impulseResponseFilePath.isEmpty()) {
        return;
    }

    const juce::File impulseResponseFile(impulseResponseFilePath);
    if (!impulseResponseFile.existsAsFile()) {
        throw std::runtime_error("Impulse Response file does not exist: " + impulseResponseFilePath.toStdString());
    }

    loadImpulseResponse(impulseResponseFile);

    juce::MessageManager::callAsync([this, impulseResponseFile] {
        if (auto* editor = dynamic_cast<ProcessorEditor*>(getActiveEditor())) {
            editor->setCurrentIRFile(impulseResponseFile);
            editor->updateIRButtonText(impulseResponseFile.getFileName());
        }
    });
}

void Processor::restoreNAMModel(const juce::ValueTree& state)
{
    const auto namModelFilePath = state.getProperty("NAM_MODEL_FILE_PATH").toString();
    if (namModelFilePath.isEmpty()) {
        return;
    }

    const juce::File namModelFile(namModelFilePath);
    if (!namModelFile.existsAsFile()) {
        throw std::runtime_error("NAM Model file does not exist: " + namModelFilePath.toStdString());
    }

    loadNAMModel(namModelFile);

    juce::MessageManager::callAsync([this, namModelFile] {
        if (auto* editor = dynamic_cast<ProcessorEditor*>(getActiveEditor())) {
            editor->setCurrentModelFile(namModelFile);
            editor->updateModelButtonText(namModelFile.getFileName());
        }
    });

}

float Processor::getRMSOutputValue() const {return rmsOutput.getCurrentValue();}
float Processor::getRMSInputValue() const {return rmsInput.getCurrentValue();}

//==============================================================================
const juce::String Processor::getName() const { return JucePlugin_Name; }
bool Processor::acceptsMidi() const { return false; }
bool Processor::producesMidi() const { return false; }
bool Processor::isMidiEffect() const { return false; }
double Processor::getTailLengthSeconds() const { return 0.0; }
int Processor::getNumPrograms() { return 1; }
int Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram(int) {}
const juce::String Processor::getProgramName(int) { return {}; }
void Processor::changeProgramName(int, const juce::String& /*newName*/) {}
bool Processor::hasEditor() const { return true; }
juce::AudioProcessorEditor* Processor::createEditor() {return new ProcessorEditor(*this);}
//==============================================================================

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {return new Processor();}



