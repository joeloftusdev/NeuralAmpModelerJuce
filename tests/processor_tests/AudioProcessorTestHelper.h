#pragma once

#define USE_MATH_DEFINES
#include "PluginProcessor.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <cmath>
#include <juce_events/juce_events.h>
#include "juce_audio_formats/codecs/flac/compat.h"


class AudioProcessorTestHelper {
public:
    AudioProcessorTestHelper()
        : sampleRate(44100.0), numSamples(static_cast<int>(sampleRate)), numChannels(2) {
        processor.prepareToPlay(sampleRate, 512);
    }

    void generateSineWave(const double frequency) {
        buffer.setSize(numChannels, numSamples);
        buffer.clear();
        for (int channel = 0; channel < numChannels; ++channel) {
            float* channelData = buffer.getWritePointer(channel);
            for (int i = 0; i < numSamples; ++i) {
                const double time = static_cast<double>(i) / sampleRate;
                channelData[i] = static_cast<float>(std::sin(2.0 * M_PI * frequency * time));
            }
        }
    }

    void processAudio() {
        juce::MidiBuffer midiMessages;
        for (int startSample = 0; startSample < numSamples; startSample += 512) {
            const int thisNumSamples = juce::jmin(512, numSamples - startSample);
            juce::AudioBuffer tempBuffer(buffer.getArrayOfWritePointers(), numChannels, startSample, thisNumSamples);
            processor.processBlock(tempBuffer, midiMessages);
        }
    }

    void setParameter(const juce::String& parameterID, const float value) {
        const auto& apvts = processor.getAPVTS();
        if (auto* param = apvts.getParameter(parameterID)) {
            param->setValueNotifyingHost(value);
        }
    }

    bool verifySignalProcessed(const juce::AudioBuffer<float>& originalBuffer) const {
        for (int channel = 0; channel < numChannels; ++channel) {
            for (int i = 0; i < numSamples; ++i) {
                if (buffer.getSample(channel, i) != originalBuffer.getSample(channel, i)) {
                    return true;
                }
            }
        }
        return false;
    }

    static float calculateAverageAmplitude(const juce::AudioBuffer<float>& buff) {
        float sum = 0.0f;
        const auto localNumSamples = buff.getNumSamples();
        const auto localNumChannels = buff.getNumChannels();
        for (int channel = 0; channel < localNumChannels; ++channel) {
            for (int i = 0; i < localNumSamples; ++i) {
                sum += std::abs(buff.getSample(channel, i));
            }
        }
        return sum / (static_cast<float>(localNumSamples) * static_cast<float>(localNumChannels));
    }

    juce::AudioBuffer<float>& getBuffer() {
        return buffer;
    }

    Processor& getProcessor() {
        return processor;
    }

private:
    Processor processor;
    juce::AudioBuffer<float> buffer;
    double sampleRate;
    int numSamples;
    int numChannels;
    juce::ScopedJuceInitialiser_GUI scopedJuceInitialiser;
};