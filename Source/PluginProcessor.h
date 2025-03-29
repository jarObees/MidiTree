/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
// Testing the thing out so that it actually copies it over.
#include <JuceHeader.h>
#include <juce_data_structures/juce_data_structures.h>
#include "LSystemProcessor.h"
#include "view.h"

//==============================================================================
class MidiArpeggiatorAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiArpeggiatorAudioProcessor();
    ~MidiArpeggiatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void getParams();

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    std::atomic<float> genParam;
    std::atomic<float> velParam;

    LSystemProcessor lsysProcessor{ genParam };

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    // Non-Automatable Values =======================================================================================
    // Defining Value Trees for non numerical parameters.
    static const inline juce::Identifier userRulesetNodeType{ "userRulesetNode" };
    juce::ValueTree userRulesetNode{ userRulesetNodeType };

    static const inline juce::Identifier userAxiomNodeType{ "userAxiomNode" };
    juce::ValueTree userAxiomNode{ userAxiomNodeType };

    //TODO: No use anywhere else.
    // Original purpose was to store a list of l systems created. But new idea is different.
    // Store lsystems as simply presets (store the inputs n shit instead of generating some new shit.)
    static const inline juce::Identifier lSystemListNodeType{ "lSystemList" };
    juce::ValueTree lSystemList{ lSystemListNodeType };

    float noteRate; // Contains float value of note fraction. (E.g 1/4 is stored as 0.25f)
    int midiAxiom; // Value representing the initial midi input by user. Should be set to -1 if no user input.
    
    static const inline juce::StringArray comboBoxNoteTypes{"Quarter", "Dotted"};
    
private:
    bool isFirstNote;
    bool isMidiHeldDown;
    bool mustTurnOff;
    float sampRate;
    int currentNote; // Index into note that is played in notesPool
    int midiNoteToPlay; // Note number of note to-be-played
    int timer; // Representts the time that has elapsed since the start of the note on output. 
    juce::SortedSet<int> notes;

    juce::ValueTree view;
    jive::Interpreter viewInterpreter;
    juce::UndoManager undoManager;
    //TODO: Should I also include the audioparameterfloats stuff?
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiArpeggiatorAudioProcessor)
};