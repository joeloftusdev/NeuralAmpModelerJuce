#include "gtest/gtest.h"
#include "../../source/state/StateManager.h"
#include "PluginProcessor.h"
#include "juce_events/juce_events.h"

class ProcessorStateTest : public ::testing::Test {
protected:
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    Processor processor;
    void SetUp() override {
        processor.prepareToPlay(44100, 512);
    }
    void TearDown() override {}
};

TEST_F(ProcessorStateTest, SaveAndRestoreState) {
    juce::MemoryBlock stateData;
    processor.getStateInformation(stateData);
    const auto& apvts = processor.getAPVTS();
    if (auto* param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("OUTPUT_ID"))) {
        param->setValueNotifyingHost(0.5f);
    }
    processor.setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));
    juce::MemoryBlock restoredStateData;
    processor.getStateInformation(restoredStateData);
    EXPECT_EQ(stateData, restoredStateData);
}

TEST_F(ProcessorStateTest, ReloadImpulseResponse) {
    const juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    const juce::File testIRFile = executableFile.getParentDirectory().getChildFile("..").getChildFile("tests/resources/test_ir.wav");
    processor.loadImpulseResponse(testIRFile);

    auto state = processor.getAPVTS().copyState();
    state.setProperty("IMPULSE_RESPONSE_FILE_PATH", testIRFile.getFullPathName(), nullptr);

    juce::MemoryBlock stateData;
    if (const auto xml = state.createXml()) {
        Processor::copyXmlToBinary(*xml, stateData);
    }

    processor.setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));
    const auto& apvts = processor.getAPVTS();
    const auto restoredIRFilePath = apvts.state.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    EXPECT_EQ(restoredIRFilePath, testIRFile.getFullPathName());
    const auto& restoredState = processor.getAPVTS().state;
    const auto restoredIRFilePathFromState = restoredState.getProperty("IMPULSE_RESPONSE_FILE_PATH").toString();
    EXPECT_EQ(restoredIRFilePathFromState, testIRFile.getFullPathName());
}

TEST_F(ProcessorStateTest, ReloadModel) {
    const juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    const juce::File modelFile = executableFile.getParentDirectory().getChildFile("..").getChildFile("tests/resources/model.nam");
    processor.loadNAMModel(modelFile);

    auto state = processor.getAPVTS().copyState();
    state.setProperty("NAM_MODEL_FILE_PATH", modelFile.getFullPathName(), nullptr);

    juce::MemoryBlock stateData;
    if (const auto xml = state.createXml()) {
        Processor::copyXmlToBinary(*xml, stateData);
    }

    processor.setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));
    const auto& apvts = processor.getAPVTS();
    const auto restoredModelFilePath = apvts.state.getProperty("NAM_MODEL_FILE_PATH").toString();
    EXPECT_EQ(restoredModelFilePath, modelFile.getFullPathName());
    const auto& restoredState = processor.getAPVTS().state;
    const auto restoredModelFilePathFromState = restoredState.getProperty("NAM_MODEL_FILE_PATH").toString();
    EXPECT_EQ(restoredModelFilePathFromState, modelFile.getFullPathName());
}