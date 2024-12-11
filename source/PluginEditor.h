#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "rms/meter.h"
#include "GUI/components/ToggleSlider.h"
#include "GUI/components/RotarySlider.h"
#include "GUI/components/RoundedRect.h"
#include "GUI/lookandfeel/ButtonLF.h"
#include "GUI/lookandfeel/LabelLF.h"
#include "GUI/lookandfeel/ToggleLF.h"

class ProcessorEditor final : public juce::AudioProcessorEditor, public juce::Timer

{
public:
    explicit ProcessorEditor (Processor&);
    ~ProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    static void drawShadowedLine(juce::Graphics& g, int startX, int startY, int endX, int endY, int shadowOffset = 2);
    void updateModelButtonText(const juce::String& text);
    void updateIRButtonText(const juce::String& text);
    void updateBypassStates();
    void setCurrentModelFile(const juce::File& file){currentModelFile = file;}
    void setCurrentIRFile(const juce::File& file){currentIRFile = file;}

    friend class ProcessorEditorGetters;

private:
    Processor& audioProcessor;
    juce::TextEditor modelPath;
    juce::TextButton loadModelButton;
    juce::TextButton loadIRButton;
    std::unique_ptr<juce::FileChooser> myChooser;

    ImageRotarySlider bassSlider;
    ImageRotarySlider midSlider;
    ImageRotarySlider trebleSlider;
    juce::Label bassLabel;
    juce::Label midLabel;
    juce::Label trebleLabel;

    ImageRotarySlider inputSlider;
    ImageRotarySlider outputSlider;
    juce::Label inputLabel;
    juce::Label outputLabel;
    juce::Label titleLabel;

    ImageRotarySlider noiseGateThresholdSlider;
    juce::Label noiseGateThresholdLabel;


    AnimatedToggleSlider bypassToneStackButton;
    juce::Label bypassToneStackLabel;
    AnimatedToggleSlider bypassNoiseGateButton;
    juce::Label bypassNoiseGateLabel;
    AnimatedToggleSlider normalizeOutputButton;
    juce::Label normalizeOutputLabel;

    juce::ToggleButton bypassIRButton;


    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassToneStackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassNoiseGateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> normalizeOutputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseGateThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassIRAttachment;


    ui::meter::Meter inputMeter;
    ui::meter::Meter outputMeter;

    juce::Image backgroundImage;

    LabelLF labelLookAndFeel;
    ButtonLF buttonLookAndFeel;
    ToggleLookAndFeel toggleLookAndFeel;

    juce::TextButton prevModelButton;
    juce::TextButton nextModelButton;
    juce::TextButton loadDirectoryButton;
    juce::File currentModelFile;

    juce::TextButton prevIRButton;
    juce::TextButton nextIRButton;
    juce::TextButton loadIRDirectoryButton;
    juce::File currentIRFile;

    juce::TextButton clearNAMButton;
    juce::TextButton clearIRButton;

    RoundedRectangleComponent namRoundedRect;
    RoundedRectangleComponent irRoundedRect;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorEditor)
};