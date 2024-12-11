#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "core/NAMProcessor.h"
#include "impulse-response/ImpulseResponse.h"
#include "state/StateManager.h"


class Processor final : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    Processor();
    ~Processor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState& getAPVTS() {
        return apvts;
    }
    const juce::ValueTree& getStoredState() const { return storedState; }


    void loadNAMModel(const juce::File &filePath);
    void clearNAM ();
    void loadImpulseResponse(const juce::File& file);
    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters ();
    void restoreImpulseResponse(const juce::ValueTree& state);
    void clearImpulseResponse();
    void restoreNAMModel(const juce::ValueTree& state);
    float getRMSOutputValue() const;
    float getRMSInputValue() const;
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    std::function<void(const juce::String&)> onModelFileChanged;

  private:
    bool namModelLoaded{false};
    bool irLoaded{false};
    juce::ValueTree storedState;
    double mSampleRate{};
    juce::uint32 mSamplesPerBlock{};
    std::string modelPath;
    bool irBypass{false};

    std::unique_ptr<nam_processor::AbstractNAMProcessor> namProcessor;
    std::unique_ptr<dsp::impulse_response::AbstractImpulseResponse> irProcessor;
    std::unique_ptr<state_manager::AbstractStateManager> stateManager;
    juce::LinearSmoothedValue<float> rmsOutput;
    juce::LinearSmoothedValue<float> rmsInput;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
};
