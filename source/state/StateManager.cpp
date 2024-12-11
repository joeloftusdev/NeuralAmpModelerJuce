
#include "StateManager.h"

namespace state_manager {

    StateManager::StateManager(juce::AudioProcessor& processor, juce::AudioProcessorValueTreeState& apvts)
        :processor(processor),apvts(apvts){}

    StateManager::~StateManager()
    = default;

    void StateManager::setStateInformation(const void* data, const int sizeInBytes) const {
        if (const std::unique_ptr xmlState(juce::AudioProcessor::getXmlFromBinary(data, sizeInBytes)); xmlState != nullptr)
            if (xmlState->hasTagName(apvts.state.getType()))
                apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
    }

    void StateManager::getStateInformation(juce::MemoryBlock& destData) const {
        const auto state = apvts.copyState();
        const std::unique_ptr xml(state.createXml());
        juce::AudioProcessor::copyXmlToBinary(*xml, destData);
    }
}