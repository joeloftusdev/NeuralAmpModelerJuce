#pragma once

#include <juce_dsp/juce_dsp.h>

namespace dsp::tone_stack
{
    class AbstractToneStack
    {
    public:
        virtual ~AbstractToneStack() = default;

        virtual void process(juce::AudioBuffer<float>& buffer) = 0;
        virtual void prepare(const juce::dsp::ProcessSpec& spec)
        {
            mSampleRate = spec.sampleRate;
            mMaxBlockSize = spec.maximumBlockSize;
        }
        virtual void setParam(const juce::String& name, float val) = 0;

    protected:
        [[nodiscard]] double getSampleRate() const { return mSampleRate; }
        double mSampleRate{};
        juce::uint32 mMaxBlockSize{};
    };

    class BasicNamToneStack final : public AbstractToneStack
    {
    public:
        BasicNamToneStack()
        {
            BasicNamToneStack::setParam("bass", 5.0f);
            BasicNamToneStack::setParam("middle", 5.0f);
            BasicNamToneStack::setParam("treble", 5.0f);
        }
        ~BasicNamToneStack() override = default;

        void process(juce::AudioBuffer<float>& buffer) override;
        void prepare(const juce::dsp::ProcessSpec& spec) override;
        void setParam(const juce::String& name, float val) override;

    private:
        void updateFilterParams();

        juce::dsp::IIR::Filter<float> mToneBass;
        juce::dsp::IIR::Filter<float> mToneMid;
        juce::dsp::IIR::Filter<float> mToneTreble;

        float mBassVal{5.0f};
        float mMiddleVal{5.0f};
        float mTrebleVal{5.0f};
    };
} // namespace dsp::tone_stack