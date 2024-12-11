#include "ImpulseResponse.h"

namespace dsp::impulse_response {
    ImpulseResponse::ImpulseResponse()= default;

    ImpulseResponse::~ImpulseResponse() = default;

    void ImpulseResponse::prepare(const juce::dsp::ProcessSpec& spec)
    {
        convolution.prepare(spec);
        clear();
    }

    void ImpulseResponse::process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> convolverBlock(buffer);
        convolution.process(juce::dsp::ProcessContextReplacing(convolverBlock));
    }

    void ImpulseResponse::setImpulseResponse(const juce::File impulseResponseFile)
    {
        if (!impulseResponseFile.existsAsFile()) {
            throw std::runtime_error("Impulse Response file does not exist: " + impulseResponseFile.getFullPathName().toStdString());
        }

        convolution.loadImpulseResponse(impulseResponseFile, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }

    void ImpulseResponse::clear()
    {
        convolution.reset();
    }

} // namespace dsp::impulse_response