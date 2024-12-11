#include "ToneStack.h"

void dsp::tone_stack::BasicNamToneStack::process(juce::AudioBuffer<float>& buffer)
{
    if (mSampleRate <= 0)
    {
        return;
    }

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData[] = { buffer.getWritePointer(channel) };
        juce::dsp::AudioBlock block(channelData, 1, buffer.getNumSamples());
        juce::dsp::ProcessContextReplacing context(block);

        mToneBass.process(context);
        mToneMid.process(context);
        mToneTreble.process(context);
    }
}

void dsp::tone_stack::BasicNamToneStack::prepare(const juce::dsp::ProcessSpec& spec)
{
    AbstractToneStack::prepare(spec);
    updateFilterParams();
}

void dsp::tone_stack::BasicNamToneStack::setParam(const juce::String& name, const float val)
{
    if (name == "bass")
    {
        mBassVal = val;
    }
    else if (name == "middle")
    {
        mMiddleVal = val;
    }
    else if (name == "treble")
    {
        mTrebleVal = val;
    }
    updateFilterParams();
}

void dsp::tone_stack::BasicNamToneStack::updateFilterParams()
{
    if (mSampleRate <= 0)
    {
        return;
    }

    const float bassGainDB = 4.0f * (mBassVal - 5.0f);
    const auto bassCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, 150.0f, 0.707f, juce::Decibels::decibelsToGain(bassGainDB));
    mToneBass.coefficients = bassCoefficients;

    const float midGainDB = 3.0f * (mMiddleVal - 5.0f);
    const float midQuality = midGainDB < 0.0f ? 1.5f : 0.7f;
    const auto midCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, 425.0f, midQuality, juce::Decibels::decibelsToGain(midGainDB));
    mToneMid.coefficients = midCoefficients;

    const float trebleGainDB = 2.0f * (mTrebleVal - 5.0f);
    const auto trebleCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(mSampleRate, 1800.0f, 0.707f, juce::Decibels::decibelsToGain(trebleGainDB));
    mToneTreble.coefficients = trebleCoefficients;
}