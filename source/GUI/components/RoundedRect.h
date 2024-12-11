#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class RoundedRectangleComponent final : public juce::Component
{
public:
    RoundedRectangleComponent()
        : shadower(shadow)
    {
        shadower.setOwner(this);
    }

    void paint(juce::Graphics& g) override
    {
        const auto bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(bounds, 6.0f);
    }

private:
    juce::DropShadow shadow { juce::Colours::grey.withAlpha(0.5f), 6, juce::Point(0, 5) };
    juce::DropShadower shadower;
};
