#include "gtest/gtest.h"
#include "PluginEditor.h"
#include "../test-helper/editor_test_helper.h"

class SliderTest : public ::testing::Test {
protected:
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    Processor processor;
    ProcessorEditor editor{processor};


    void SetUp() override {
        editor.setSize(600, 400);
    }

    void TearDown() override{
    }
};

TEST_F(SliderTest, InitialValues) {
    EXPECT_EQ(ProcessorEditorGetters::getBassSlider(editor).getValue(), 5.0);
    EXPECT_EQ(ProcessorEditorGetters::getMidSlider(editor).getValue(), 5.0);
    EXPECT_EQ(ProcessorEditorGetters::getTrebleSlider(editor).getValue(), 5.0);
    EXPECT_EQ(ProcessorEditorGetters::getInputSlider(editor).getValue(), 0.5);
    EXPECT_EQ(ProcessorEditorGetters::getOutputSlider(editor).getValue(), 0.5);
    EXPECT_EQ(ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).getValue(), -20.0);
}

TEST_F(SliderTest, Range) {
    EXPECT_EQ(ProcessorEditorGetters::getBassSlider(editor).getMinimum(), 0.0);
    EXPECT_EQ(ProcessorEditorGetters::getBassSlider(editor).getMaximum(), 10.0);
    EXPECT_EQ(ProcessorEditorGetters::getMidSlider(editor).getMinimum(), 0.0);
    EXPECT_EQ(ProcessorEditorGetters::getMidSlider(editor).getMaximum(), 10.0);
    EXPECT_EQ(ProcessorEditorGetters::getTrebleSlider(editor).getMinimum(), 0.0);
    EXPECT_EQ(ProcessorEditorGetters::getTrebleSlider(editor).getMaximum(), 10.0);
    EXPECT_EQ(ProcessorEditorGetters::getInputSlider(editor).getMinimum(), -20.0);
    EXPECT_EQ(ProcessorEditorGetters::getInputSlider(editor).getMaximum(), 20.0);
    EXPECT_EQ(ProcessorEditorGetters::getOutputSlider(editor).getMinimum(), -40.0);
    EXPECT_EQ(ProcessorEditorGetters::getOutputSlider(editor).getMaximum(), 40.0);
    EXPECT_EQ(ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).getMinimum(), -40.0);
    EXPECT_EQ(ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).getMaximum(), 0.0);
}

TEST_F(SliderTest, ValueChange) {
    ProcessorEditorGetters::getBassSlider(editor).setValue(7.0);
    EXPECT_EQ(ProcessorEditorGetters::getBassSlider(editor).getValue(), 7.0);

    ProcessorEditorGetters::getMidSlider(editor).setValue(3.0);
    EXPECT_EQ(ProcessorEditorGetters::getMidSlider(editor).getValue(), 3.0);

    ProcessorEditorGetters::getTrebleSlider(editor).setValue(8.0);
    EXPECT_EQ(ProcessorEditorGetters::getTrebleSlider(editor).getValue(), 8.0);

    ProcessorEditorGetters::getInputSlider(editor).setValue(-10.0);
    EXPECT_EQ(ProcessorEditorGetters::getInputSlider(editor).getValue(), -10.0);

    ProcessorEditorGetters::getOutputSlider(editor).setValue(15.0);
    EXPECT_EQ(ProcessorEditorGetters::getOutputSlider(editor).getValue(), 15.0);

    ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).setValue(-20.0);
    EXPECT_EQ(ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).getValue(), -20.0);
}

TEST_F(SliderTest, TextValueSuffix) {
    EXPECT_EQ(ProcessorEditorGetters::getInputSlider(editor).getTextValueSuffix(), " dB");
    EXPECT_EQ(ProcessorEditorGetters::getOutputSlider(editor).getTextValueSuffix(), " dB");
    EXPECT_EQ(ProcessorEditorGetters::getNoiseGateThresholdSlider(editor).getTextValueSuffix(), " dB");
}