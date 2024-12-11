#pragma once

#include <juce_dsp/juce_dsp.h>

namespace dsp::noise_gate
{
    class AbstractNoiseGateProcessor
    {
    public:
        virtual ~AbstractNoiseGateProcessor() = default;

        virtual void prepare(const juce::dsp::ProcessSpec& spec) = 0;
        virtual void process(juce::AudioBuffer<float>& audioBuffer) = 0;
        virtual void setThreshold(float val) = 0;
        virtual void setRatio(float ratio) = 0;
        virtual void setAttack(float attack) = 0;
        virtual void setRelease(float release) = 0;

    protected:
        double mSampleRate{};
        juce::uint32 mMaxBlockSize{};
        juce::uint32 mNumChannels{};
    };

    class NoiseGateProcessor final : public AbstractNoiseGateProcessor
    {
    public:
        NoiseGateProcessor() = default;

        void prepare(const juce::dsp::ProcessSpec& spec) override;
        void process(juce::AudioBuffer<float>& audioBuffer) override;

        void setThreshold(const float val) override { mThresholdValue = val; mNoiseGate.setThreshold(val); }
        void setRatio(const float ratio) override { mNoiseGate.setRatio(ratio); }
        void setAttack(const float attack) override { mNoiseGate.setAttack(attack); }
        void setRelease(const float release) override { mNoiseGate.setRelease(release); }

    private:
        juce::dsp::NoiseGate<float> mNoiseGate;
        float mThresholdValue{0.0f};
    };
} // namespace dsp::noise_gate