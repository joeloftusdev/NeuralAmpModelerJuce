#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "GUI/components/RotarySlider.h"
#include "GUI/components/ToggleSlider.h"

class ProcessorEditor;
class ProcessorEditorGetters {
public:
    static juce::File getCurrentIRFile(const ProcessorEditor& editor);
    static juce::File getCurrentModelFile(const ProcessorEditor& editor);
    static juce::TextButton& getLoadModelButton(ProcessorEditor& editor);
    static juce::TextButton& getLoadIRButton(ProcessorEditor& editor);
    static juce::TextButton& getClearNAMButton(ProcessorEditor& editor);
    static juce::TextButton& getClearIRButton(ProcessorEditor& editor);
    static juce::TextButton& getPrevIRButton(ProcessorEditor& editor);
    static juce::TextButton& getNextIRButton(ProcessorEditor& editor);
    static juce::TextButton& getNextModelButton(ProcessorEditor& editor);
    static juce::TextButton& getPrevModelButton(ProcessorEditor& editor);
    static juce::TextButton& getLoadModelDirectoryButton(ProcessorEditor& editor);
    static juce::TextButton& getLoadIRDirectoryButton(ProcessorEditor& editor);
    static ImageRotarySlider& getInputSlider(ProcessorEditor& editor);
    static ImageRotarySlider& getOutputSlider(ProcessorEditor& editor);
    static ImageRotarySlider& getBassSlider(ProcessorEditor& editor);
    static ImageRotarySlider& getMidSlider(ProcessorEditor& editor);
    static ImageRotarySlider& getTrebleSlider(ProcessorEditor& editor);
    static ImageRotarySlider& getNoiseGateThresholdSlider(ProcessorEditor& editor);
    static AnimatedToggleSlider& getBypassToneStackButton(ProcessorEditor& editor);
    static AnimatedToggleSlider& getBypassNoiseGateButton(ProcessorEditor& editor);
    static juce::ToggleButton& getBypassIRButton(ProcessorEditor& editor);
};