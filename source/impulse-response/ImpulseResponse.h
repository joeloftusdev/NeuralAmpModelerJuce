#pragma once

#include <juce_dsp/juce_dsp.h>


namespace dsp::impulse_response {
    class AbstractImpulseResponse
    {
    public:
        virtual ~AbstractImpulseResponse() = default;

        virtual void prepare(const juce::dsp::ProcessSpec& spec) = 0;
        virtual void process(juce::AudioBuffer<float>& buffer) = 0;
        virtual void setImpulseResponse(juce::File impulseResponseFile) = 0;
        virtual void clear() = 0;
    };

    class ImpulseResponse final : public AbstractImpulseResponse
    {
    public:

        ImpulseResponse();
        ~ImpulseResponse() override;

        void process(juce::AudioBuffer<float>& buffer) override;
        void prepare(const juce::dsp::ProcessSpec& spec) override;
        void setImpulseResponse(juce::File impulseResponseFile) override;
        void clear() override;

    private:
        juce::dsp::Convolution convolution;
    };
} // namespace dsp::impulse_response