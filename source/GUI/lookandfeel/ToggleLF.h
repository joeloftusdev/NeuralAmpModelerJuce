#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ToggleLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    ToggleLookAndFeel()
    {
        const auto svgData = BinaryData::IRIcon_svg;
        constexpr auto svgDataSize = BinaryData::IRIcon_svgSize;
        svgImage = juce::Drawable::createFromImageData(svgData, svgDataSize);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
    {
        if (svgImage != nullptr)
        {
            const auto bounds = button.getLocalBounds().toFloat();
            const juce::Colour colour = button.getToggleState() ? juce::Colours::darkgrey : juce::Colours::white;

            const auto svgCopy = svgImage->createCopy();
            if (const auto* drawableComposite = dynamic_cast<juce::DrawableComposite*>(svgCopy.get()))
            {
                for (auto* child : drawableComposite->getChildren())
                {
                    if (auto* drawablePath = dynamic_cast<juce::DrawablePath*>(child))
                    {
                        drawablePath->setFill(colour);
                    }
                }
            }

            svgCopy->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.0f);
        }
    }

    juce::MouseCursor getMouseCursorFor(juce::Component& /*component*/) override
    {
        return juce::MouseCursor::PointingHandCursor;
    }

private:
    std::unique_ptr<juce::Drawable> svgImage;
};