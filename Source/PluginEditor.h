/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class MidiArpeggiatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidiArpeggiatorAudioProcessorEditor (MidiArpeggiatorAudioProcessor&);
    ~MidiArpeggiatorAudioProcessorEditor() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiArpeggiatorAudioProcessor& audioProcessor;
    juce::TextEditor inputUserRuleset;
    juce::TextEditor inputUserAxiom;
    std::unique_ptr<juce::AlertWindow> alertWindow;

    juce::TextButton generateButton{ "Generate!" };
    
    void generateButtonClick();
    bool addUserRuleset(std::string ruleSet);
    bool checkUserAxiom(std::string axiomInput);
    void throwCustomError(std::string errorMessage);
    
    juce::SortedSet<std::string> lsysVariables;
    juce::SortedSet<std::string> lsysRulesets;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiArpeggiatorAudioProcessorEditor)
};
