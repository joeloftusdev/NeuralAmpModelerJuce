#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

namespace state_manager
{
    class AbstractStateManager
    {
    public:
        virtual ~AbstractStateManager() = default;

        virtual void setStateInformation(const void* data, int sizeInBytes) const = 0;
        virtual void getStateInformation(juce::MemoryBlock& destData) const = 0;
        [[nodiscard]] virtual juce::AudioProcessorValueTreeState& getAPVTS() const = 0;
    };

    class StateManager final : public AbstractStateManager
    {
    public:
        StateManager(juce::AudioProcessor& processor, juce::AudioProcessorValueTreeState& apvts);
        ~StateManager() override;

        void setStateInformation(const void* data, int sizeInBytes) const override;
        void getStateInformation(juce::MemoryBlock& destData) const override;
        [[nodiscard]] juce::AudioProcessorValueTreeState& getAPVTS() const override {
            return apvts;
        }

    private:
        juce::AudioProcessor& processor;
        juce::AudioProcessorValueTreeState& apvts;
    };
}