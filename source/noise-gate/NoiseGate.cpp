#include "NoiseGate.h"

namespace dsp::noise_gate
{
        void NoiseGateProcessor::prepare(const juce::dsp::ProcessSpec& spec)
        {
            mSampleRate = spec.sampleRate;
            mMaxBlockSize = spec.maximumBlockSize;
            mNumChannels = spec.numChannels;
            mNoiseGate.prepare(spec);
        }

    void NoiseGateProcessor::process(juce::AudioBuffer<float>& audioBuffer) {
            juce::dsp::AudioBlock<float> block(audioBuffer);
            const juce::dsp::ProcessContextReplacing context(block);
            mNoiseGate.process(context);
        }
}; // namespace dsp::noise_gate
