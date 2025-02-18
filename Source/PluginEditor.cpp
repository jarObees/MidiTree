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

// TODO: Prevent conflicting rules. Such as 1=21, 1=32
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
    lsysVariablesDisplay = tempLsysVariables;
    lsysRulesetsDisplay = tempLsysRulesets;
    return true;
}

// Checks if axiom is in the variables, and returns appropriate bool if match is found.
bool MidiArpeggiatorAudioProcessorEditor::checkUserAxiom(std::string axiomInput)
{
    for (auto it = lsysVariablesDisplay.begin(); it != lsysVariablesDisplay.end(); ++it)
    {
        if (*it == axiomInput)
        {
            lsysAxiomDisplay = axiomInput;
            return true;
        }
    }
    throwCustomError("Axiom is not a valid variable!");
    return false;
}

void MidiArpeggiatorAudioProcessorEditor::generateButtonClick()
{
    std::string userRuleset = inputUserRuleset.getText().toStdString();
    std::string userAxiom = inputUserAxiom.getText().toStdString();

    // TODO: Rename variables better?
    // Attempts to store user ruleset and axiom
    if (addUserRuleset(userRuleset) == true && checkUserAxiom(userAxiom) == true)
    {
        audioProcessor.lsysProcessor.current_lsysVars = lsysVariablesDisplay;
        audioProcessor.lsysProcessor.current_lsysRulesets = lsysRulesetsDisplay;
        audioProcessor.lsysProcessor.current_axiom = lsysAxiomDisplay;
        audioProcessor.lsysProcessor.generateLSystem(static_cast<uint8_t>(audioProcessor.genParam.load()));
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
        };
    // Sets text to data from value tree.
    if (audioProcessor.userRulesetNode.hasProperty("userRulesetNode"))
    {
        inputUserRuleset.setText(audioProcessor.userRulesetNode.getProperty("userRulesetNode"));
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

    // GEN SLIDERS
    genSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    genSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    genAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, "gens", genSlider);
    generateButton.onClick = [this]() {generateButtonClick(); };
    addAndMakeVisible(genSlider);

    //NOTE DIVISION SLIDER
    noteDivSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    noteDivSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 70, 40);
    noteDivAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.apvts, "rate", noteDivSlider);
    addAndMakeVisible(noteDivSlider);
    
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
}

void MidiArpeggiatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    inputUserRuleset.setBounds(10, 10, getWidth() - 300, getHeight() - 100);
    generateButton.setBounds(100, 10, 100, 40);
    inputUserAxiom.setBounds(100, 80, 100, 40);
    genSlider.setBounds(130, 160, 100, 40);
    noteDivSlider.setBounds(230, 260, 100, 40);
}