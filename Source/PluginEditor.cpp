/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <regex>


// Throws a custom error message to the user
// We use std::make_unique so that we can further customize the alert window if needed.
void MidiArpeggiatorAudioProcessorEditor::throwCustomError(std::string errorMessage)
{
    alertWindow = std::make_unique<juce::AlertWindow>("placeHolder",
                                                      "placeHolder",
                                                      juce::MessageBoxIconType::WarningIcon,
                                                      nullptr);
    alertWindow->showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Alert!!",errorMessage,"ok",nullptr);
}

// Checks each line to make sure that it follows correct format and adds. 
bool MidiArpeggiatorAudioProcessorEditor::addUserRuleset(std::string ruleSetInput)
{
    std::regex rulesetPattern("^[b#]?[1-7]=([b#]?[1-7])+$"); // Used to capture strings like, "1=#2b4", "4=2b34#4", etc...
    std::regex variablePattern("[b#]?[1-7]"); // Individually captures strings like, "1", "#2", "b7"
    
    // Iterates through and verifies each line in ruleSetInput.
    std::stringstream ss(ruleSetInput); 
    std::string line;
    juce::SortedSet<std::string> tempLsysVariables;
    juce::SortedSet<std::string> tempLsysRulesets;
    while (std::getline(ss, line))
    {
        // If the line is valid, adds it to the sorted set.
        if (std::regex_match(line, rulesetPattern))
        {
            tempLsysRulesets.add(line);
            auto begin = std::sregex_iterator(line.begin(), line.end(), variablePattern);
            auto end = std::sregex_iterator();
            for (auto it = begin; it != end; ++it)
            {
                tempLsysVariables.add(it->str());
            }
        }
        else
        {
            throwCustomError("Incorrect Ruleset Format.");
            return false;
        }
    }
    lsysVariables = tempLsysVariables;
    lsysRulesets = tempLsysRulesets;
    return true;
}

// Checks if axiom is in the variables, and returns appropriate bool if match is found.
bool MidiArpeggiatorAudioProcessorEditor::checkUserAxiom(std::string axiomInput)
{
    for (auto it = lsysVariables.begin(); it != lsysVariables.end(); ++it)
    {
        if (*it == axiomInput) return true;
    }
    throwCustomError("Axiom is not a valid variable!");
    return false;
}

void MidiArpeggiatorAudioProcessorEditor::generateButtonClick()
{
    auto juceStringRuleset = inputUserRuleset.getText();
    auto juceStringAxiom = inputUserAxiom.getText();

    std::string userRuleset = juceStringRuleset.toStdString();
    std::string userAxiom = juceStringAxiom.toStdString();

    if (addUserRuleset(userRuleset) == true && checkUserAxiom(userAxiom) == true)
    {
        //DO THE THING.
        DBG("OVERALL SUCCESS---");
    }
    else
    {
        DBG("OVERALL FAILURE---");
    }
}
//==============================================================================
MidiArpeggiatorAudioProcessorEditor::MidiArpeggiatorAudioProcessorEditor (MidiArpeggiatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 400);
    inputUserRuleset.setMultiLine(true);
    inputUserRuleset.setReturnKeyStartsNewLine(true);
    inputUserRuleset.setTextToShowWhenEmpty("1=12 \n 2=1 \n ...", juce::Colours::black); // TODO: String doesn't show up properly.
    inputUserRuleset.setFont(juce::Font(15.0));
    inputUserRuleset.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
    inputUserRuleset.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    inputUserRuleset.setColour(juce::TextEditor::outlineColourId, juce::Colours::black);
    // On text change, sends data to value tree.
    inputUserRuleset.onTextChange = [this]()
        {
            audioProcessor.userRulesetNode.setProperty("userRulesetNode", inputUserRuleset.getText(), nullptr);
            DBG("Text changed: rulesetNode set to text...");
        };
    // Sets text to data from value tree.
    if (audioProcessor.userRulesetNode.hasProperty("userRulesetNode"))
    {
        inputUserRuleset.setText(audioProcessor.userRulesetNode.getProperty("userRulesetNode"));
        DBG("Retrieving text from rulesetNode...");
    }

    inputUserAxiom.setFont(juce::Font(15.0));
    inputUserAxiom.setTextToShowWhenEmpty("Axiom", juce::Colours::black);
    inputUserAxiom.setInputRestrictions(1);
    inputUserAxiom.onTextChange = [this]()
        {
            audioProcessor.userAxiomNode.setProperty("userAxiomNode", inputUserAxiom.getText(), nullptr);
        };
    if (audioProcessor.userAxiomNode.hasProperty("userAxiomNode"))
    {
        inputUserAxiom.setText(audioProcessor.userAxiomNode.getProperty("userAxiomNode"));
    }

    generateButton.onClick = [this]() {generateButtonClick(); };


    addAndMakeVisible(inputUserRuleset);
    addAndMakeVisible(generateButton);
    addAndMakeVisible(inputUserAxiom);
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
    inputUserRuleset.setBounds(10, 10, getWidth() - 300, getHeight() - 100);
    generateButton.setBounds(100, 10, 100, 40);
    inputUserAxiom.setBounds(100, 80, 100, 40);
}
