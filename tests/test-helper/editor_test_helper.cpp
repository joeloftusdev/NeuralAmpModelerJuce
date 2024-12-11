#include "editor_test_helper.h"
#include "PluginEditor.h"

juce::File ProcessorEditorGetters::getCurrentIRFile(const ProcessorEditor& editor) { return editor.currentIRFile; }
juce::File ProcessorEditorGetters::getCurrentModelFile(const ProcessorEditor& editor) { return editor.currentModelFile; }
juce::TextButton& ProcessorEditorGetters::getLoadModelButton(ProcessorEditor& editor) { return editor.loadModelButton; }
juce::TextButton& ProcessorEditorGetters::getLoadIRButton(ProcessorEditor& editor) { return editor.loadIRButton; }
juce::TextButton& ProcessorEditorGetters::getClearNAMButton(ProcessorEditor& editor) { return editor.clearNAMButton; }
juce::TextButton& ProcessorEditorGetters::getClearIRButton(ProcessorEditor& editor) { return editor.clearIRButton; }
juce::TextButton& ProcessorEditorGetters::getPrevIRButton(ProcessorEditor& editor) { return editor.prevIRButton; }
juce::TextButton& ProcessorEditorGetters::getNextIRButton(ProcessorEditor& editor) { return editor.nextIRButton; }
juce::TextButton& ProcessorEditorGetters::getNextModelButton(ProcessorEditor& editor) { return editor.nextModelButton; }
juce::TextButton& ProcessorEditorGetters::getPrevModelButton(ProcessorEditor& editor) { return editor.prevModelButton; }
juce::TextButton& ProcessorEditorGetters::getLoadModelDirectoryButton(ProcessorEditor& editor) { return editor.loadDirectoryButton; }
juce::TextButton& ProcessorEditorGetters::getLoadIRDirectoryButton(ProcessorEditor& editor) { return editor.loadIRDirectoryButton; }
ImageRotarySlider& ProcessorEditorGetters::getInputSlider(ProcessorEditor& editor) { return editor.inputSlider; }
ImageRotarySlider& ProcessorEditorGetters::getOutputSlider(ProcessorEditor& editor) { return editor.outputSlider; }
ImageRotarySlider& ProcessorEditorGetters::getBassSlider(ProcessorEditor& editor) { return editor.bassSlider; }
ImageRotarySlider& ProcessorEditorGetters::getMidSlider(ProcessorEditor& editor) { return editor.midSlider; }
ImageRotarySlider& ProcessorEditorGetters::getTrebleSlider(ProcessorEditor& editor) { return editor.trebleSlider; }
ImageRotarySlider& ProcessorEditorGetters::getNoiseGateThresholdSlider(ProcessorEditor& editor) { return editor.noiseGateThresholdSlider; }
AnimatedToggleSlider& ProcessorEditorGetters::getBypassToneStackButton(ProcessorEditor& editor) { return editor.bypassToneStackButton; }
AnimatedToggleSlider& ProcessorEditorGetters::getBypassNoiseGateButton(ProcessorEditor& editor) { return editor.bypassNoiseGateButton; }
juce::ToggleButton& ProcessorEditorGetters::getBypassIRButton(ProcessorEditor& editor) { return editor.bypassIRButton; }