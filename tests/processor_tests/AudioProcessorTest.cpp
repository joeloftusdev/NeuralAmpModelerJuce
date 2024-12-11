#include "gtest/gtest.h"
#include "PluginProcessor.h"
#include "juce_events/juce_events.h"
#include "AudioProcessorTestHelper.h"

class AudioProcessorIntegrationTest : public ::testing::Test {
protected:
    juce::ScopedJuceInitialiser_GUI juceInitialiser;
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AudioProcessorIntegrationTest, ParameterInitializationTest) {
    Processor processor;
    const auto& apvts = processor.getAPVTS();
    EXPECT_TRUE(apvts.getParameter("INPUT_ID") != nullptr);
    EXPECT_TRUE(apvts.getParameter("OUTPUT_ID") != nullptr);
}


TEST_F(AudioProcessorIntegrationTest, AudioProcessingTest) {
    AudioProcessorTestHelper helper;
    helper.generateSineWave(440.0);

    const juce::AudioBuffer<float> originalBuffer = helper.getBuffer();

    helper.setParameter("INPUT", 0.5f);

    helper.processAudio();

    EXPECT_TRUE(helper.verifySignalProcessed(originalBuffer));
}

TEST_F(AudioProcessorIntegrationTest, GainParameterTest) {
    AudioProcessorTestHelper helper;
    helper.generateSineWave(440.0);

    helper.processAudio();
    const auto originalAmplitude = AudioProcessorTestHelper::calculateAverageAmplitude(helper.getBuffer());

    helper.setParameter("OUTPUT_ID", juce::Decibels::decibelsToGain(35.0f));

    helper.generateSineWave(440.0);

    helper.processAudio();
    const auto increasedAmplitude = AudioProcessorTestHelper::calculateAverageAmplitude(helper.getBuffer());

    EXPECT_GT(increasedAmplitude, originalAmplitude);
}


TEST_F(AudioProcessorIntegrationTest, LoadModelAndCheckEffect) {
    AudioProcessorTestHelper helper;
    const juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    const juce::File modelFile = executableFile.getParentDirectory().getChildFile("..").getChildFile("tests/resources/model.nam");

    helper.getProcessor().loadNAMModel(modelFile);

    helper.generateSineWave(440.0);

    helper.processAudio();
    const auto originalBuffer = helper.getBuffer();

    helper.getProcessor().loadNAMModel(modelFile);

    helper.processAudio();
    const auto processedBuffer = helper.getBuffer();

    EXPECT_TRUE(helper.verifySignalProcessed(originalBuffer));
}

TEST_F(AudioProcessorIntegrationTest, LoadIRAndCheckEffect) {
    AudioProcessorTestHelper helper;
    const juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    const juce::File IRFile = executableFile.getParentDirectory().getChildFile("..").getChildFile("tests/resources/test_ir.wav");

    helper.getProcessor().loadImpulseResponse(IRFile);

    helper.generateSineWave(440.0);

    helper.processAudio();
    const auto originalBuffer = helper.getBuffer();

    helper.getProcessor().loadImpulseResponse(IRFile);

    helper.processAudio();
    const auto processedBuffer = helper.getBuffer();

    EXPECT_TRUE(helper.verifySignalProcessed(originalBuffer));
}

TEST_F(AudioProcessorIntegrationTest, BypassIRTest) {
    AudioProcessorTestHelper helper;
    const juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    const juce::File IRFile = executableFile.getParentDirectory().getChildFile("..").getChildFile("tests/resources/test_ir.wav");

    helper.generateSineWave(440.0);
    helper.processAudio();
    const auto bufferWithoutIR = helper.getBuffer();

    helper.getProcessor().loadImpulseResponse(IRFile);
    helper.processAudio();
    const auto bufferWithIR = helper.getBuffer();

    bool isProcessed = false;
    for (int channel = 0; channel < bufferWithoutIR.getNumChannels(); ++channel) {
        for (int i = 0; i < bufferWithoutIR.getNumSamples(); ++i) {
            if (bufferWithoutIR.getSample(channel, i) != bufferWithIR.getSample(channel, i)) {
                isProcessed = true;
                break;
            }
        }
        if (isProcessed) break;
    }

    EXPECT_TRUE(isProcessed);
}

