/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <regex>

void MidiArpeggiatorAudioProcessorEditor::handleUserRulesetClick()
{
    auto juceString = userRulesetInput.getText();
    // Used to capture strings like, "1=#2b4", "4=2b34#4", etc...
    std::regex pattern("^[b#]?[1-7]=([b#]?[1-7])+$");

    std::string userInput = juceString.toStdString();
    std::stringstream ss(userInput);
    std::string line;
    while (std::getline(ss, line))
    {
        if (std::regex_match(line, pattern))
        {
            DBG("YEAH THIS MATCHES!!! LESS GOOO");
        }
        else
        {
            DBG("nope");
        }
    }
}
// TO:DO Fix string in setTextToShowWhenEmpty
//==============================================================================
MidiArpeggiatorAudioProcessorEditor::MidiArpeggiatorAudioProcessorEditor (MidiArpeggiatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 400);
    userRulesetInput.setMultiLine(true);
    userRulesetInput.setReturnKeyStartsNewLine(true);
    userRulesetInput.setTextToShowWhenEmpty("1=12 \n 2=1 \n ...", juce::Colours::black);
    userRulesetInput.setFont(juce::Font(15.0));
    userRulesetInput.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
    userRulesetInput.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    userRulesetInput.setColour(juce::TextEditor::outlineColourId, juce::Colours::black);

    inputUserRulesetButton.onClick = [this]() {handleUserRulesetClick(); };

    addAndMakeVisible(userRulesetInput);
    addAndMakeVisible(inputUserRulesetButton);
}

MidiArpeggiatorAudioProcessorEditor::~MidiArpeggiatorAudioProcessorEditor()
{

}

//==============================================================================
void MidiArpeggiatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!!", getLocalBounds(), juce::Justification::centred, 1);
}

void MidiArpeggiatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    userRulesetInput.setBounds(10, 10, getWidth() - 300, getHeight() - 100);
    inputUserRulesetButton.setBounds(40, 10, 100, 40);
}
