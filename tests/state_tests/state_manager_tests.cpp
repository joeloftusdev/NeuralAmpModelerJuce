#include "gtest/gtest.h"
#include "../../source/state/StateManager.h"
#include "PluginProcessor.h"
#include "juce_events/juce_events.h"

class StateManagerTest : public ::testing::Test {
protected:
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    Processor processor;
    std::unique_ptr<state_manager::StateManager> stateManager;

    void SetUp() override {
        stateManager = std::make_unique<state_manager::StateManager>(processor, processor.getAPVTS());
    }

    void TearDown() override {
        stateManager.reset();
    }
};

TEST_F(StateManagerTest, SaveAndRestoreState) {
    juce::MemoryBlock stateData;
    stateManager->getStateInformation(stateData);

    const auto& apvts = processor.getAPVTS();
    if (auto* param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("OUTPUT_ID"))) {
        param->setValueNotifyingHost(0.5f);
    }

    stateManager->setStateInformation(stateData.getData(), static_cast<int>(stateData.getSize()));
    juce::MemoryBlock restoredStateData;
    stateManager->getStateInformation(restoredStateData);
    EXPECT_EQ(stateData, restoredStateData);
}


