#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class LabelLF final : public juce::LookAndFeel_V4
{
public:
    LabelLF()
    {
        fontData = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf, BinaryData::RobotoRegular_ttfSize);
    }


    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        const auto bounds = label.getLocalBounds();

        g.setColour(label.findColour(juce::Label::backgroundColourId));
        g.fillRect(bounds);

        g.setColour(juce::Colours::lightgrey);
        g.setFont(fontData);

        const juce::String text = label.getText();
        g.drawText(text, bounds, juce::Justification::centred, false);
    }

private:
    juce::Font fontData;
};
