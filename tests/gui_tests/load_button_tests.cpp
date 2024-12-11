#include "gtest/gtest.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../test-helper/editor_test_helper.h"
#include "juce_events/juce_events.h"


class LoadingTests : public ::testing::Test {
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

TEST_F(LoadingTests, LoadModelButtonTest) {
    ProcessorEditorGetters::getLoadModelButton(*editor).onClick();
    processor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", "test_model.nam", nullptr);
    const auto modelFilePath = processor.apvts.state.getProperty("NAM_MODEL_FILE_PATH").toString();
    EXPECT_FALSE(modelFilePath.isEmpty());
}

TEST_F(LoadingTests, LoadModelDirectoryTest) {
    ProcessorEditorGetters::getLoadModelDirectoryButton(*editor).onClick();
    processor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", "test_model.nam", nullptr);
    const auto modelFilePath = processor.apvts.state.getProperty("NAM_MODEL_FILE_PATH").toString();
    EXPECT_FALSE(modelFilePath.isEmpty());
}

TEST_F(LoadingTests, LoadIRButtonTest) {
    ProcessorEditorGetters::getLoadIRButton(*editor).onClick();
    processor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", "test_ir.wav", nullptr);
    const auto irFilePath = processor.apvts.state.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    EXPECT_FALSE(irFilePath.isEmpty());
}

TEST_F(LoadingTests, LoadIRDirectoryTest) {
    ProcessorEditorGetters::getLoadIRDirectoryButton(*editor).onClick();
    processor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", "test_ir.wav", nullptr);
    const auto irFilePath = processor.apvts.state.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    EXPECT_FALSE(irFilePath.isEmpty());
}

TEST_F(LoadingTests, ClearNAMButtonTest) {
    processor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", "test_model.nam", nullptr);
    ProcessorEditorGetters::getClearNAMButton(*editor).onClick();
    const auto modelFilePath = processor.apvts.state.getProperty("NAM_MODEL_FILE_PATH").toString();
    EXPECT_TRUE(modelFilePath.isEmpty());
}

TEST_F(LoadingTests, ClearIRButtonTest) {
    processor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", "test_ir.wav", nullptr);
    ProcessorEditorGetters::getClearIRButton(*editor).onClick();
    const auto irFilePath = processor.apvts.state.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    EXPECT_TRUE(irFilePath.isEmpty());
}
