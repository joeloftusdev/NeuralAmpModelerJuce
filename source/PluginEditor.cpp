#include "PluginProcessor.h"
#include "PluginEditor.h"


ProcessorEditor::ProcessorEditor (Processor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
     inputMeter([&]() {return audioProcessor.getRMSInputValue();}),
     outputMeter([&]() {return audioProcessor.getRMSOutputValue();})
{

    setSize(600, 400);
    setResizable(false, true);
    getConstrainer()->setFixedAspectRatio(1.5);

    setResizeLimits(400, 300, 1200, 900);

    addAndMakeVisible(namRoundedRect);
    addAndMakeVisible(irRoundedRect);

    addAndMakeVisible(loadModelButton);
    loadModelButton.setLookAndFeel(&buttonLookAndFeel);
    loadModelButton.setButtonText("Select Model...");
    loadModelButton.onClick = [this] {
        if (currentModelFile.existsAsFile()) {
            const auto directory = currentModelFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.nam");

            juce::PopupMenu menu;
            for (int i = 0; i < files.size(); ++i) {
                menu.addItem(i + 1, files[i].getFileName());
            }

            menu.showMenuAsync(juce::PopupMenu::Options(), [this, files](const int result) {
                if (result > 0) {
                    const auto& file = files[result - 1];
                    audioProcessor.loadNAMModel(file);
                    audioProcessor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", file.getFullPathName(), nullptr);
                    currentModelFile = file;
                    loadModelButton.setButtonText(file.getFileName());
                }
            });
        } else {
            myChooser = std::make_unique<juce::FileChooser>(
                "Select a file to open...", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.nam");

            myChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                                   [this](const juce::FileChooser &chooser) {
                                       if (const auto file = chooser.getResult(); file.existsAsFile()) {
                                           audioProcessor.loadNAMModel(file);
                                           audioProcessor.apvts.state.setProperty(
                                               "NAM_MODEL_FILE_PATH", file.getFullPathName(), nullptr);
                                           currentModelFile = file;
                                           loadModelButton.setButtonText(file.getFileName());
                                       }
                                   });
        }
    };

    addAndMakeVisible(loadDirectoryButton);
    loadDirectoryButton.setLookAndFeel(&buttonLookAndFeel);
    loadDirectoryButton.setButtonText("DIR");
    loadDirectoryButton.onClick = [this] {
        myChooser = std::make_unique<juce::FileChooser>(
            "Select a file to open...", juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.nam");

        myChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                               [this](const juce::FileChooser &chooser) {
                                   if (const auto file = chooser.getResult(); file.existsAsFile()) {
                                       audioProcessor.loadNAMModel(file);
                                       audioProcessor.apvts.state.setProperty(
                                           "NAM_MODEL_FILE_PATH", file.getFullPathName(), nullptr);
                                       currentModelFile = file;
                                       loadModelButton.setButtonText(file.getFileName());
                                   }
                               });
    };

    addAndMakeVisible(prevModelButton);
    prevModelButton.setLookAndFeel(&buttonLookAndFeel);
    prevModelButton.setButtonText("<");
    prevModelButton.onClick = [this] {
        if (currentModelFile.existsAsFile()) {
            const auto directory = currentModelFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.nam");
            if (auto it = std::find(files.begin(), files.end(), currentModelFile); it != files.end() && it != files.begin()) {
                --it;
                audioProcessor.loadNAMModel(*it);
                audioProcessor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", it->getFullPathName(), nullptr);
                currentModelFile = *it;
                loadModelButton.setButtonText(it->getFileName());
            }
        }
    };

    addAndMakeVisible(nextModelButton);
    nextModelButton.setLookAndFeel(&buttonLookAndFeel);
    nextModelButton.setButtonText(">");
    nextModelButton.onClick = [this] {
        if (currentModelFile.existsAsFile()) {
            const auto directory = currentModelFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.nam");
            if (auto it = std::find(files.begin(), files.end(), currentModelFile); it != files.end() && std::next(it) != files.end()) {
                ++it;
                audioProcessor.loadNAMModel(*it);
                audioProcessor.apvts.state.setProperty("NAM_MODEL_FILE_PATH", it->getFullPathName(), nullptr);
                currentModelFile = *it;
                loadModelButton.setButtonText(it->getFileName());
            }
        }
    };

    addAndMakeVisible(loadIRButton);
    loadIRButton.setLookAndFeel(&buttonLookAndFeel);
    loadIRButton.setButtonText("Select IR...");
    loadIRButton.onClick = [this] {
        if (currentIRFile.existsAsFile()) {
            const auto directory = currentIRFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.wav");

            juce::PopupMenu menu;
            for (int i = 0; i < files.size(); ++i) {
                menu.addItem(i + 1, files[i].getFileName());
            }

            menu.showMenuAsync(juce::PopupMenu::Options(), [this, files](const int result) {
                if (result > 0) {
                    const auto &file = files[result - 1];
                    audioProcessor.loadImpulseResponse(file);
                    audioProcessor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", file.getFullPathName(),
                                                           nullptr);
                    currentIRFile = file;
                    loadIRButton.setButtonText(file.getFileName());
                }
            });
        } else {
            myChooser = std::make_unique<juce::FileChooser>(
                "Select an impulse response file to open...",
                juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.wav");

            myChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                                   [this](const juce::FileChooser &chooser) {
                                       if (const auto file = chooser.getResult(); file.existsAsFile()) {
                                           audioProcessor.loadImpulseResponse(file);
                                           audioProcessor.apvts.state.setProperty(
                                               "IMPULSE_RESPONSE_FILE_PATH", file.getFullPathName(), nullptr);
                                           currentIRFile = file;
                                           loadIRButton.setButtonText(file.getFileName());
                                       }
                                   });
        }
    };

    addAndMakeVisible(prevIRButton);
    prevIRButton.setLookAndFeel(&buttonLookAndFeel);
    prevIRButton.setButtonText("<");
    prevIRButton.onClick = [this] {
        if (currentIRFile.existsAsFile()) {
            const auto directory = currentIRFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.wav");
            if (auto it = std::find(files.begin(), files.end(), currentIRFile);
                it != files.end() && it != files.begin()) {
                --it;
                audioProcessor.loadImpulseResponse(*it);
                audioProcessor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", it->getFullPathName(), nullptr);
                currentIRFile = *it;
                loadIRButton.setButtonText(it->getFileName());
            }
        }
    };

    addAndMakeVisible(nextIRButton);
    nextIRButton.setLookAndFeel(&buttonLookAndFeel);
    nextIRButton.setButtonText(">");
    nextIRButton.onClick = [this] {
        if (currentIRFile.existsAsFile()) {
            const auto directory = currentIRFile.getParentDirectory();
            auto files = directory.findChildFiles(juce::File::findFiles, false, "*.wav");
            if (auto it = std::find(files.begin(), files.end(), currentIRFile);
                it != files.end() && std::next(it) != files.end()) {
                ++it;
                audioProcessor.loadImpulseResponse(*it);
                audioProcessor.apvts.state.setProperty("IMPULSE_RESPONSE_FILE_PATH", it->getFullPathName(), nullptr);
                currentIRFile = *it;
                loadIRButton.setButtonText(it->getFileName());
            }
        }
    };

    addAndMakeVisible(loadIRDirectoryButton);
    loadIRDirectoryButton.setLookAndFeel(&buttonLookAndFeel);
    loadIRDirectoryButton.setButtonText("DIR");
    loadIRDirectoryButton.onClick = [this] {
        myChooser = std::make_unique<juce::FileChooser>(
            "Select an impulse response file to open...",
            juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.wav");

        myChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                               [this](const juce::FileChooser &chooser) {
                                   if (const auto file = chooser.getResult(); file.existsAsFile()) {
                                       audioProcessor.loadImpulseResponse(file);
                                       audioProcessor.apvts.state.setProperty(
                                           "IMPULSE_RESPONSE_FILE_PATH", file.getFullPathName(), nullptr);
                                       currentIRFile = file;
                                       loadIRButton.setButtonText(file.getFileName());
                                   }
                               });
    };

    addAndMakeVisible(clearNAMButton);
    clearNAMButton.setLookAndFeel(&buttonLookAndFeel);
    clearNAMButton.setButtonText("X");
    clearNAMButton.onClick = [this] {
        audioProcessor.clearNAM();
        audioProcessor.apvts.state.removeProperty("NAM_MODEL_FILE_PATH", nullptr);
        loadModelButton.setButtonText("Select Model...");
        currentModelFile = juce::File();
    };

    addAndMakeVisible(clearIRButton);
    clearIRButton.setLookAndFeel(&buttonLookAndFeel);
    clearIRButton.setButtonText("X");
    clearIRButton.onClick = [this] {
        audioProcessor.clearImpulseResponse();
        audioProcessor.apvts.state.removeProperty("IMPULSE_RESPONSE_FILE_PATH", nullptr);
        loadIRButton.setButtonText("Select IR...");
        currentIRFile = juce::File();
    };


    addAndMakeVisible(bassSlider);
    bassSlider.setRange(0.0, 10.0, 0.01);
    bassSlider.setValue(5.0);


    addAndMakeVisible(midSlider);
    midSlider.setRange(0.0, 10.0, 0.01);
    midSlider.setValue(5.0);

    addAndMakeVisible(trebleSlider);
    trebleSlider.setRange(0.0, 10.0, 0.01);
    trebleSlider.setValue(5.0);

    addAndMakeVisible(inputSlider);
    inputSlider.setRange(-20.0, 20.0, 0.01);
    inputSlider.setValue(5.0);
    inputSlider.setTextValueSuffix(" dB");


    addAndMakeVisible(outputSlider);
    outputSlider.setRange(-40.0, 40.0, 0.01);
    outputSlider.setValue(5.0);
    outputSlider.setTextValueSuffix(" dB");

    addAndMakeVisible(noiseGateThresholdSlider);
    noiseGateThresholdSlider.setRange(-40.0, 0.0, 0.01);
    noiseGateThresholdSlider.setTextValueSuffix(" dB");

    bypassToneStackButton.onClick = [this] { updateBypassStates(); };
    bypassNoiseGateButton.onClick = [this] { updateBypassStates(); };


    addAndMakeVisible(bypassToneStackButton);
    bypassToneStackButton.setButtonText("Bypass EQ");
    bypassToneStackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "TONE_STACK_BYPASS_ID", bypassToneStackButton);
    addAndMakeVisible(bypassToneStackLabel);
    bypassToneStackLabel.setText("EQ", juce::dontSendNotification);
    bypassToneStackLabel.setJustificationType(juce::Justification::centred);
    bypassNoiseGateLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(bypassNoiseGateButton);
    bypassNoiseGateButton.setButtonText("Bypass Gate");
    bypassNoiseGateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "NOISE_GATE_BYPASS_ID", bypassNoiseGateButton);
    addAndMakeVisible(bypassNoiseGateLabel);
    bypassNoiseGateLabel.setText("Noise Gate", juce::dontSendNotification);
    bypassNoiseGateLabel.setJustificationType(juce::Justification::centred);
    bypassNoiseGateLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(normalizeOutputButton);
    normalizeOutputButton.setButtonText("Normalize");
    normalizeOutputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "NORMALIZE_OUTPUT_ID", normalizeOutputButton);
    addAndMakeVisible(normalizeOutputLabel);
    normalizeOutputLabel.setText("Normalize", juce::dontSendNotification);
    normalizeOutputLabel.setJustificationType(juce::Justification::centred);
    normalizeOutputLabel.setLookAndFeel(&labelLookAndFeel);

    bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "BASS_ID", bassSlider);
    midAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "MIDDLE_ID", midSlider);
    trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "TREBLE_ID", trebleSlider);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "INPUT_ID", inputSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "OUTPUT_ID", outputSlider);
    noiseGateThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "NOISE_GATE_THRESHOLD_ID", noiseGateThresholdSlider);

    addAndMakeVisible(bassLabel);
    bassLabel.setText("Bass", juce::dontSendNotification);
    bassLabel.attachToComponent(&bassSlider, false);
    bassLabel.setJustificationType(juce::Justification::centred);
    bassLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(midLabel);
    midLabel.setText("Mid", juce::dontSendNotification);
    midLabel.attachToComponent(&midSlider, false);
    midLabel.setJustificationType(juce::Justification::centred);
    midLabel.setLookAndFeel(&labelLookAndFeel);


    addAndMakeVisible(trebleLabel);
    trebleLabel.setText("Treble", juce::dontSendNotification);
    trebleLabel.attachToComponent(&trebleSlider, false);
    trebleLabel.setJustificationType(juce::Justification::centred);
    trebleLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(inputLabel);
    inputLabel.setText("Input", juce::dontSendNotification);
    inputLabel.attachToComponent(&inputSlider, false);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputSlider, false);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(noiseGateThresholdLabel);
    noiseGateThresholdLabel.setText("Threshold", juce::dontSendNotification);
    noiseGateThresholdLabel.attachToComponent(&noiseGateThresholdSlider, false);
    noiseGateThresholdLabel.setJustificationType(juce::Justification::centred);
    noiseGateThresholdLabel.setLookAndFeel(&labelLookAndFeel);

    addAndMakeVisible(titleLabel);
    titleLabel.setText("NAM JUCE", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    const auto titleFont = juce::Typeface::createSystemTypefaceFor(BinaryData::MichromaRegular_ttf, BinaryData::MichromaRegular_ttfSize);
    juce::FontOptions fontOptions;
    fontOptions = fontOptions.withStyle("").withTypeface(titleFont).withHeight(40.0f);
    titleLabel.setFont(juce::Font(fontOptions));

    bypassIRButton.onClick = [this] {
        updateBypassStates();
    };

    bypassIRButton.setButtonText("Bypass IR");
    bypassIRButton.setLookAndFeel(&toggleLookAndFeel);
    addAndMakeVisible(bypassIRButton);

    bypassIRAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "IR_BYPASS", bypassIRButton);


    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    startTimerHz(24);

    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);

    updateBypassStates();
}

ProcessorEditor::~ProcessorEditor()
{
    noiseGateThresholdLabel.setLookAndFeel(nullptr);
    bypassNoiseGateLabel.setLookAndFeel(nullptr);
    bypassToneStackLabel.setLookAndFeel(nullptr);
    normalizeOutputLabel.setLookAndFeel(nullptr);
    bassLabel.setLookAndFeel(nullptr);
    midLabel.setLookAndFeel(nullptr);
    trebleLabel.setLookAndFeel(nullptr);
    inputLabel.setLookAndFeel(nullptr);
    outputLabel.setLookAndFeel(nullptr);
    loadModelButton.setLookAndFeel(nullptr);
    loadIRButton.setLookAndFeel(nullptr);
    loadDirectoryButton.setLookAndFeel(nullptr);
    bypassIRButton.setLookAndFeel(nullptr);
}

void ProcessorEditor::drawShadowedLine(juce::Graphics& g, const int startX, const int startY, const int endX, const int endY, const int shadowOffset)
{
    g.setColour(juce::Colours::darkgrey.withAlpha(0.4f));
    g.drawLine(static_cast<float>(startX), static_cast<float>(startY + shadowOffset), static_cast<float>(endX), static_cast<float>(endY + shadowOffset), 2.0f);
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.drawLine(static_cast<float>(startX), static_cast<float>(startY), static_cast<float>(endX), static_cast<float>(endY), 2.0f);
}

void ProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }

    const auto area = getLocalBounds();
    const int margin = area.getWidth() / 100;
    const int titleHeight = area.getHeight() / 6;

    const int titleLineWidth = (area.getWidth() * 3) / 4;
    drawShadowedLine(g, (area.getWidth() - titleLineWidth) / 2, margin + titleHeight + 10, (area.getWidth() + titleLineWidth) / 2, margin + titleHeight + 10);

    const int midSliderLineY = midSlider.getBottom() + 20;
    drawShadowedLine(g, bassSlider.getX() + bassSlider.getWidth() / 2, midSliderLineY, trebleSlider.getX() + trebleSlider.getWidth() / 2, midSliderLineY);

    const int noiseGateLineY = noiseGateThresholdSlider.getBottom() + 20;
    drawShadowedLine(g, noiseGateThresholdSlider.getX(), noiseGateLineY, noiseGateThresholdSlider.getRight(), noiseGateLineY);
}



void ProcessorEditor::resized()
{
    const auto area = getLocalBounds();
    constexpr int numSliders = 6;
    const int meterWidth = area.getWidth() / 30;
    const int margin = area.getWidth() / 100;
    const int topPadding = static_cast<int>(area.getHeight() / 3.5);
    const int sliderSize = juce::jmin((area.getWidth() - meterWidth * 2 - margin * (numSliders + 1)) / numSliders, area.getHeight() / 5);
    const int sliderY = margin + topPadding;

    const int titleHeight = area.getHeight() / 5;

    titleLabel.setBounds(margin, margin, area.getWidth() - margin * 2, titleHeight);

    const int meterY = static_cast<int>(area.getHeight() * 0.2);
    const int meterHeight = static_cast<int>(area.getHeight() * 0.47);

    const int inputMeterX = static_cast<int>(area.getWidth() * 0.028);
    const int outputMeterX = static_cast<int>(area.getWidth() * 0.972 - meterWidth);

    inputMeter.setBounds(inputMeterX, meterY, meterWidth, meterHeight);
    outputMeter.setBounds(outputMeterX, meterY, meterWidth, meterHeight);

    const int reducedMargin = margin / 10;
    const int totalSliderWidth = (sliderSize * numSliders) + (reducedMargin * (numSliders - 1));
    const int startX = (area.getWidth() - meterWidth * 2 - totalSliderWidth) / 2 + meterWidth;

    for (int i = 0; i < numSliders; ++i)
    {
        const auto sliderX = startX + i * (sliderSize + reducedMargin);
        auto& slider = [&]() -> juce::Slider& {
            switch (i)
            {
                case 0: return inputSlider;
                case 1: return noiseGateThresholdSlider;
                case 2: return bassSlider;
                case 3: return midSlider;
                case 4: return trebleSlider;
                case 5: return outputSlider;
                default: ;
            }
            return inputSlider;
        }();
        slider.setBounds(sliderX, sliderY, sliderSize, sliderSize);
    }

    const int buttonHeight = area.getHeight() / 20;
    const int buttonWidth = buttonHeight * 2;
    const int buttonYOffset = margin;

    auto alignButtonToSlider = [&](const juce::Slider& slider, AnimatedToggleSlider& button, juce::Label& label) {
        const auto sliderBounds = slider.getBounds();
        button.setBounds(
            sliderBounds.getCentreX() - buttonWidth / 2,
            sliderY + sliderSize + buttonYOffset * 6,
            buttonWidth + margin,
            buttonHeight - (margin / 2)
        );

        constexpr int labelHeight = 20;
        const int labelWidth = button.getWidth() * 2;
        label.setBounds(
            button.getX() + (button.getWidth() - labelWidth) / 2,
            button.getBottom() + 5,
            labelWidth,
            labelHeight
        );
    };

    alignButtonToSlider(noiseGateThresholdSlider, bypassNoiseGateButton, bypassNoiseGateLabel);
    alignButtonToSlider(midSlider, bypassToneStackButton, bypassToneStackLabel);
    alignButtonToSlider(outputSlider, normalizeOutputButton, normalizeOutputLabel);

    const int loadButtonHeight = area.getHeight() / 20;
    const int loadButtonYOffset = margin;
    const int navButtonWidth = buttonHeight;
    const int loadButtonWidth = (area.getWidth() - margin * 2) * 3 / 6;
    const int totalNavButtonsWidth = navButtonWidth * 3;
    const int totalWidth = totalNavButtonsWidth + loadButtonWidth;
    const int loadButtonX = static_cast<int>(area.getWidth() * 0.5 - totalWidth * 0.5);
    const int loadButtonY = static_cast<int>(area.getHeight() * 0.77);
    const int clearButtonWidth = navButtonWidth;


    const int rectX = loadButtonX - margin / 2;
    const int rectY = loadButtonY - margin / 2;
    const int rectWidth = totalWidth + clearButtonWidth + margin;
    const int rectHeight = loadButtonHeight + margin;

    namRoundedRect.setBounds(rectX, rectY, rectWidth, rectHeight);

    loadDirectoryButton.setBounds(loadButtonX, loadButtonY, navButtonWidth, loadButtonHeight);
    prevModelButton.setBounds(loadButtonX + navButtonWidth, loadButtonY, navButtonWidth, loadButtonHeight);
    nextModelButton.setBounds(loadButtonX + navButtonWidth * 2, loadButtonY, navButtonWidth, loadButtonHeight);
    loadModelButton.setBounds(loadButtonX + totalNavButtonsWidth, loadButtonY, loadButtonWidth, loadButtonHeight);
    clearNAMButton.setBounds(loadButtonX + totalNavButtonsWidth + loadButtonWidth, loadButtonY, clearButtonWidth,
                             loadButtonHeight);

    constexpr int increasedGap = 10;
    const int secondRectX = loadButtonX - margin / 2;
    const int secondRectY = loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap - margin / 2;
    const int secondRectWidth = totalWidth + clearButtonWidth + margin;
    const int secondRectHeight = loadButtonHeight + margin;

    const int bypassIRButtonWidth = buttonWidth;
    const int bypassIRButtonHeight = loadButtonHeight;
    const int bypassIRButtonX = loadButtonX - bypassIRButtonWidth - margin;
    const int bypassIRButtonY = loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap;

    bypassIRButton.setBounds(bypassIRButtonX, bypassIRButtonY, bypassIRButtonWidth, bypassIRButtonHeight);

    irRoundedRect.setBounds(secondRectX, secondRectY, secondRectWidth, secondRectHeight);

    loadIRButton.setBounds(loadButtonX + totalNavButtonsWidth,
                           loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap, loadButtonWidth,
                           loadButtonHeight);
    prevIRButton.setBounds(loadButtonX + navButtonWidth,
                           loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap, navButtonWidth,
                           loadButtonHeight);
    nextIRButton.setBounds(loadButtonX + navButtonWidth * 2,
                           loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap, navButtonWidth,
                           loadButtonHeight);
    loadIRDirectoryButton.setBounds(loadButtonX, loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap,
                                    navButtonWidth, loadButtonHeight);
    clearIRButton.setBounds(loadButtonX + totalNavButtonsWidth + loadButtonWidth,
                            loadButtonY + loadButtonHeight + loadButtonYOffset * 2 + increasedGap, clearButtonWidth,
                            loadButtonHeight);



    repaint();
}

void ProcessorEditor::timerCallback()
{
        outputMeter.setLevel(audioProcessor.getRMSOutputValue());
        outputMeter.repaint();
        inputMeter.setLevel(audioProcessor.getRMSInputValue());
        inputMeter.repaint();
}

void ProcessorEditor::updateModelButtonText(const juce::String& text)
{
    juce::MessageManager::callAsync([this, text] {
        loadModelButton.setButtonText(text);
    });
}

void ProcessorEditor::updateIRButtonText(const juce::String& text)
{
    juce::MessageManager::callAsync([this, text] {
        loadIRButton.setButtonText(text);
    });
}


void ProcessorEditor::updateBypassStates()
{
    const bool isToneStackBypassed = bypassToneStackButton.getToggleState();
    const bool isNoiseGateBypassed = bypassNoiseGateButton.getToggleState();

    bassSlider.setEnabled(isToneStackBypassed);
    midSlider.setEnabled(isToneStackBypassed);
    trebleSlider.setEnabled(isToneStackBypassed);
    noiseGateThresholdSlider.setEnabled(isNoiseGateBypassed);

    bassSlider.setAlpha(isToneStackBypassed ? 1.0f : 0.5f);
    midSlider.setAlpha(isToneStackBypassed ? 1.0f : 0.5f);
    trebleSlider.setAlpha(isToneStackBypassed ? 1.0f : 0.5f);
    noiseGateThresholdSlider.setAlpha(isNoiseGateBypassed ? 1.0f : 0.5f);

    const bool isIRBypassed = bypassIRButton.getToggleState();

    loadIRButton.setAlpha(isIRBypassed ? 0.5f : 1.0f);
    prevIRButton.setAlpha(isIRBypassed ? 0.5f : 1.0f);
    nextIRButton.setAlpha(isIRBypassed ? 0.5f : 1.0f);
    loadIRDirectoryButton.setAlpha(isIRBypassed ? 0.5f : 1.0f);
    clearIRButton.setAlpha(isIRBypassed ? 0.5f : 1.0f);

    loadIRButton.setEnabled(!isIRBypassed);
    prevIRButton.setEnabled(!isIRBypassed);
    nextIRButton.setEnabled(!isIRBypassed);
    loadIRDirectoryButton.setEnabled(!isIRBypassed);
    clearIRButton.setEnabled(!isIRBypassed);

}


