#pragma once

#include "PluginProcessor.h"

namespace ui::meter {

    class Meter final : public juce::Component, public juce::Timer
    {
    public:
        explicit Meter(std::function<float()>&& valueFunction)
            : mValueSupplier(std::move(valueFunction)), shadower(shadow)
        {
            shadower.setOwner(this);
            startTimerHz(24);
        }

        ~Meter() override { stopTimer(); }

        void paint(juce::Graphics& g) override
        {
            const auto meterLevel = mValueSupplier();
            auto bounds = getLocalBounds().toFloat();
            constexpr float padding = 6.0f;

            juce::Path backgroundPath;
            backgroundPath.addRoundedRectangle(bounds, 2.0f);

            g.setColour(juce::Colours::black);
            g.fillPath(backgroundPath);

            const auto scaledY = juce::jmap(meterLevel, -60.0f, 6.0f, 0.0f, bounds.getHeight());
            auto barBounds = bounds.removeFromBottom(scaledY);

            barBounds.reduce(padding, 0);

            g.setColour(juce::Colour(0xFF5986DF).withAlpha(0.5f));
            g.fillRect(barBounds);

            g.setColour(juce::Colour(0xFF5986DF));
            g.drawLine(barBounds.getX(), barBounds.getBottom(), barBounds.getRight(), barBounds.getBottom(), 1.0f);

            if (scaledY > 0.0f)
            {
                g.drawLine(barBounds.getX(), barBounds.getY(), barBounds.getRight(), barBounds.getY(), 1.0f);
            }
        }

        void resized() override {}

        void setLevel(const float value) { level = value; }

        void timerCallback() override { repaint(); }

    private:
        std::function<float()> mValueSupplier;
        float level = -60.0f;

        juce::DropShadow shadow { juce::Colours::grey.withAlpha(0.2f), 3, juce::Point(5, 1) };
        juce::DropShadower shadower;
    };
}