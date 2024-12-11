#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ButtonLF final : public juce::LookAndFeel_V4
{
public:
    ButtonLF() {
        prevArrowIcon = juce::Drawable::createFromSVG(*juce::parseXML(prevArrowSvg));
        nextArrowIcon = juce::Drawable::createFromSVG(*juce::parseXML(nextArrowSvg));
        deleteIcon = juce::Drawable::createFromSVG(*juce::parseXML(deleteSvg));
        folderIcon = juce::Drawable::createFromSVG(*juce::parseXML(folderSvg));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& /*backgroundColour*/,
                              const bool isMouseOverButton, const bool isButtonDown) override
    {
        const auto bounds = button.getLocalBounds().toFloat();
        const auto baseColour = isButtonDown ? juce::Colours::black.darker() : (isMouseOverButton ? juce::Colours::black.brighter(0.2f) : juce::Colours::black);

        if (isMouseOverButton)
        {
            button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        }
        else
        {
            button.setMouseCursor(juce::MouseCursor::NormalCursor);
        }

        g.setColour(baseColour);
        g.fillRect(bounds);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool /*isHighlighted*/, bool /*isDown*/) override
    {
        const auto bounds = button.getLocalBounds();
        constexpr int padding = 5;
        const auto paddedBounds = bounds.reduced(padding);

        g.setColour(juce::Colours::lightgrey);
        g.setFont(fontData);

        if (button.getButtonText().containsIgnoreCase("X"))
        {
            drawSVGIcon(g, *deleteIcon, paddedBounds);
        }
        else if (button.getButtonText().containsIgnoreCase("DIR"))
        {
            drawSVGIcon(g, *folderIcon, paddedBounds);
        }
        else if (button.getButtonText().containsIgnoreCase("<"))
        {
            drawSVGIcon(g, *prevArrowIcon, paddedBounds);
        }
        else if (button.getButtonText().containsIgnoreCase(">"))
        {
            drawSVGIcon(g, *nextArrowIcon, paddedBounds);
        }
        else
        {
            g.drawText(button.getButtonText(), bounds, juce::Justification::centred, true);
        }
    }



private:
    std::unique_ptr<juce::Drawable> prevArrowIcon;
    std::unique_ptr<juce::Drawable> nextArrowIcon;
    std::unique_ptr<juce::Drawable> deleteIcon;
    std::unique_ptr<juce::Drawable> folderIcon;
    juce::Font fontData {juce::FontOptions{juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf, BinaryData::RobotoRegular_ttfSize)}};

    static constexpr auto nextArrowSvg = R"(
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg" fill="#bfbfbf" stroke="#bfbfbf">
            <polyline fill="none" stroke="#a9a9a9" stroke-width="2" points="7 2 17 12 7 22"></polyline>
        </svg>
    )";

    static constexpr auto prevArrowSvg = R"(
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg" fill="#bfbfbf" stroke="#bfbfbf">
            <polyline fill="none" stroke="#a9a9a9" stroke-width="2" points="17 2 7 12 17 22"></polyline>
        </svg>
    )";

    static constexpr auto deleteSvg = R"(
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg" fill="#bfbfbf" stroke="#bfbfbf">
            <line x1="18" y1="6" x2="6" y2="18" stroke="#a9a9a9" stroke-width="2"></line>
            <line x1="6" y1="6" x2="18" y2="18" stroke="#a9a9a9" stroke-width="2"></line>
        </svg>
    )";

    static constexpr auto folderSvg = R"(
        <svg viewBox="0 0 24 24" fill="none"
            xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier"
            stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round"
            stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier">
            <path d="M3 8.2C3 7.07989 3 6.51984 3.21799 6.09202C3.40973 5.71569 3.71569
                5.40973 4.09202 5.21799C4.51984 5 5.0799 5 6.2 5H9.67452C10.1637 5 10.4083 5 10.6385
                5.05526C10.8425 5.10425 11.0376 5.18506 11.2166 5.29472C11.4184 5.4184 11.5914 5.59135
                11.9373 5.93726L12.0627 6.06274C12.4086 6.40865 12.5816 6.5816 12.7834 6.70528C12.9624
                6.81494 13.1575 6.89575 13.3615 6.94474C13.5917 7 13.8363 7 14.3255 7H17.8C18.9201 7 19.4802
                7 19.908 7.21799C20.2843 7.40973 20.5903 7.71569 20.782 8.09202C21 8.51984 21 9.0799 21
                10.2V15.8C21
                16.9201 21 17.4802 20.782 17.908C20.5903 18.2843 20.2843 18.5903 19.908 18.782C19.4802 19
                18.9201 19 17.8
                19H6.2C5.07989 19 4.51984 19 4.09202 18.782C3.71569 18.5903 3.40973 18.2843 3.21799 17.908C3
                17.4802 3 16.9201
                3 15.8V8.2Z" stroke="#a9a9a9" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            </path> </g>
        </svg>
    )";

    static void drawSVGIcon(juce::Graphics& g, juce::Drawable& icon, const juce::Rectangle<int>& bounds)
    {
        icon.setTransformToFit(bounds.toFloat(), juce::RectanglePlacement::centred);
        icon.draw(g, 1.0f);
    }


};