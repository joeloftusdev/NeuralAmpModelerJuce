#include "gtest/gtest.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../test-helper/editor_test_helper.h"
#include "juce_events/juce_events.h"

class BypassStateTests : public ::testing::Test {
protected:
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    Processor processor;
    std::unique_ptr<ProcessorEditor> editor;

    void SetUp() override {
        editor = std::make_unique<ProcessorEditor>(processor);
    }

    void TearDown() override {
        editor.reset();
    }
};

TEST_F(BypassStateTests, ToneStackBypass) {
    ProcessorEditorGetters::getBypassToneStackButton(*editor).setToggleState(true, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_TRUE(ProcessorEditorGetters::getBassSlider(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getMidSlider(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getTrebleSlider(*editor).isEnabled());

    ProcessorEditorGetters::getBypassToneStackButton(*editor).setToggleState(false, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_FALSE(ProcessorEditorGetters::getBassSlider(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getMidSlider(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getTrebleSlider(*editor).isEnabled());
}

TEST_F(BypassStateTests, NoiseGateBypass) {
    ProcessorEditorGetters::getBypassNoiseGateButton(*editor).setToggleState(true, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_TRUE(ProcessorEditorGetters::getNoiseGateThresholdSlider(*editor).isEnabled());

    ProcessorEditorGetters::getBypassNoiseGateButton(*editor).setToggleState(false, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_FALSE(ProcessorEditorGetters::getNoiseGateThresholdSlider(*editor).isEnabled());
}

TEST_F(BypassStateTests, IRBypass) {
    ProcessorEditorGetters::getBypassIRButton(*editor).setToggleState(false, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_TRUE(ProcessorEditorGetters::getLoadIRButton(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getPrevIRButton(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getNextIRButton(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getLoadIRDirectoryButton(*editor).isEnabled());
    EXPECT_TRUE(ProcessorEditorGetters::getClearIRButton(*editor).isEnabled());

    ProcessorEditorGetters::getBypassIRButton(*editor).setToggleState(true, juce::dontSendNotification);
    editor->updateBypassStates();
    EXPECT_FALSE(ProcessorEditorGetters::getLoadIRButton(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getPrevIRButton(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getNextIRButton(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getLoadIRDirectoryButton(*editor).isEnabled());
    EXPECT_FALSE(ProcessorEditorGetters::getClearIRButton(*editor).isEnabled());
}