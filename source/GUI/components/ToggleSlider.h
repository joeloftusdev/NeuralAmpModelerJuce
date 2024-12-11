#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class AnimatedToggleSlider final : public juce::Button, juce::Timer
{
public:
    AnimatedToggleSlider () : Button({}) { setClickingTogglesState(true); }

private:
    void buttonStateChanged () override { startTimer(30); }

    void timerCallback() override
    {
        auto rate = 0.3f;
        rate *= getToggleState() ? 1.0f : -1.0f;

        position = juce::jlimit(0.0f, 1.0f, position + rate);

        if (position == 0.0f || position == 1.0f)
            stopTimer();

        repaint();
    }

    void paintButton (juce::Graphics& g, const bool isMouseOverButton, bool /*isButtonDown*/) override
    {
        const auto h = static_cast<float>(getHeight());
        const auto area = getLocalBounds().toFloat();

        juce::Path p;
        p.addRoundedRectangle(area, h / 2.0f);

        juce::Colour baseColour = juce::Colours::black.interpolatedWith(juce::Colour(0xFF5986DF), position * position);
        if (isMouseOverButton)
        {
            baseColour = baseColour.brighter(0.2f);
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
        }
        else
        {
            setMouseCursor(juce::MouseCursor::NormalCursor);
        }

        g.setColour(baseColour);
        g.fillPath(p);

        const auto circleBounds = area.withWidth(h).translated((area.getWidth() - h) * position, 0.0f);

        const juce::Colour ellipseBaseColour = juce::Colours::black;
        const juce::Colour ellipseHighlightColour = juce::Colours::darkgrey;

        const juce::ColourGradient ellipseGradient(
            ellipseHighlightColour, circleBounds.getCentreX(), circleBounds.getY(),
            ellipseBaseColour, circleBounds.getCentreX(), circleBounds.getBottom(),
            false
        );

        g.setGradientFill(ellipseGradient);
        g.fillEllipse(circleBounds.reduced(1.0f));

        g.setColour(juce::Colours::black.withAlpha(0.8f));
        g.drawEllipse(circleBounds.reduced(1.0f), 1.0f);
    }


    float position{ 0.0f };
};