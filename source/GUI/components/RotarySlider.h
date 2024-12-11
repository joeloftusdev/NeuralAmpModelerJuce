#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class ImageRotarySlider final : public juce::Slider
{
public:
    ImageRotarySlider()
    {
        setSliderStyle(RotaryVerticalDrag);
        setTextBoxStyle(TextBoxBelow, true, 100, 10);
        setColour(textBoxTextColourId, juce::Colours::lightgrey);
        setColour(textBoxOutlineColourId, juce::Colour(0, 0, static_cast<juce::uint8>(0), static_cast<juce::uint8>(0)));
        backgroundImage = juce::ImageCache::getFromMemory(BinaryData::knob_background_png, BinaryData::knob_background_pngSize);
    }

    juce::String getTextFromValue(const double value) override
    {
        return juce::String(value, 1) + getTextValueSuffix();
    }

void paint(juce::Graphics& g) override
{
    const auto bounds = getLocalBounds().toFloat();
    const auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 4.0f;
    const auto center = bounds.getCentre();
    const auto angle = juce::jmap(static_cast<float>(getValue()), static_cast<float>(getMinimum()), static_cast<float>(getMaximum()), juce::MathConstants<float>::pi * 1.25f, juce::MathConstants<float>::pi * 2.75f);

    g.drawImage(backgroundImage, bounds);

    const auto dotRadius = radius * 0.45f;
    const auto dotX = center.x + dotRadius * std::cos(angle - juce::MathConstants<float>::pi * 0.5f);
    const auto dotY = center.y + dotRadius * std::sin(angle - juce::MathConstants<float>::pi * 0.5f);

        juce::Path circle;
        const auto outerRadius = radius - 8.0f;
        const auto outerDiameter = outerRadius * 2.0f;
        const auto outerRx = center.x - outerRadius;
        const auto outerRy = center.y - outerRadius;
        circle.addArc(outerRx, outerRy, outerDiameter, outerDiameter, juce::MathConstants<float>::pi * 1.25f, angle, true);

    const juce::ColourGradient circleGradient(juce::Colour(0xFF5986DF).brighter(0.4f), center.x, center.y - outerRadius,
                                              juce::Colour(0xFF5986DF).darker(0.2f), center.x, center.y + outerRadius, false);
        g.setGradientFill(circleGradient);
        g.strokePath(circle, juce::PathStrokeType(2.0f));

    const juce::ColourGradient gradient(juce::Colour(0xFF5986DF).brighter(0.4f), dotX, dotY, juce::Colour(0xFF5986DF).darker(0.2f), dotX, dotY + 4.0f, true);
        g.setGradientFill(gradient);
        g.fillEllipse(dotX - 3.0f, dotY - 3.0f, 6.0f, 6.0f);
        g.setColour(juce::Colours::black);
        g.drawEllipse(dotX - 3.0f, dotY - 3.0f, 6.0f, 6.0f, 1.0f);
}

private:
    juce::Image backgroundImage;
};