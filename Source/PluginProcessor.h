/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
// Testing the thing out so that it actually copies it over.
#include <JuceHeader.h>
#include <juce_data_structures/juce_data_structures.h>
#include "View.h"
#include "LSystemProcessor.h"
#include "PresetManager.h"
#include "ForestManager.h"
#include "LSystemManager.h"
#include "DummyAudioProcessor.h"
#include <chrono>

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

    void getAutomatableParams();

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

    // Non auto params =======================================================================================================

    std::atomic<float> genParam; // IS THE ONLY AUTOMATABLE VAR THAT SHOULDN'T REALLY BE.
    std::atomic<float> velParam;
    std::atomic<float> noteType; // 0 for quarterNote, 1 for dottedNote.
	
	// ==============================================================================================================
    std::vector<std::string> noteRateKeys;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

    Dummy::DummyAudioProcessor dummyAudioProcessor{};
    juce::AudioProcessorValueTreeState::ParameterLayout createNonAutoParamaterLayout(int numRow, int numColumn);
    juce::AudioProcessorValueTreeState nonAutoApvts{ dummyAudioProcessor, 
        nullptr, 
        "dummyParams",
        createNonAutoParamaterLayout(jiveGui::AnalogUserInput::NUMBLOCKROWS, jiveGui::AnalogUserInput::NUMBLOCKCOLUMNS)};


    float noteRate; // Contains float value of note fraction. (E.g 1/4 is stored as 0.25f)
    int midiAxiom; // Value representing the initial midi input by user. Should be set to -1 if no user input.
    
    static const inline juce::StringArray comboBoxNoteTypes{"Quarter", "Dotted"};
    juce::Array<int> currentNotesPool; // DO NOT MODIFY IN PROCESS BLOCK.

    Preset::PresetManager presetManager{ apvts };
    Forest::ForestManager forestManager{ apvts, presetManager, currentNotesPool };
    LSystemStuff::LSystemManager lSystemManager{ apvts, presetManager, currentNotesPool };
private:
    int time;
    static inline const std::unordered_map<std::string, float> noteRateMap = 
    {
        {"32/1", 32.0f}, {"16/1", 16.0f},
        {"8/1", 8.0f}, {"4/1", 4.0f},
        {"2/1", 2.0f}, {"1/1", 1.0f},
        {"1/2", 0.5f}, {"1/4", 0.25f},
        {"1/8", 0.125f}, {"1/16", 0.0625f},
        {"1/32", 0.03125f}, {"1/64", 0.015625f}
    };
    std::unordered_map<std::string, juce::Image> imageCollection;
    std::unordered_map<std::string, juce::Image> getImages();
    bool isFirstNote;
    bool isMidiHeldDown;
    bool mustTurnOff;
    float sampRate;
    int notesPoolIndex; // Index into note that is played in notesPool
    int lastNoteValue; // Note number of note to-be-played
    int timer; // Representts the time that has elapsed since the start of the note on output. 
    juce::SortedSet<int> notes;

    juce::ValueTree view;
    jive::Interpreter viewInterpreter;
    juce::UndoManager undoManager;
    //TODO: Should I also include the audioparameterfloats stuff?
    // 
    int countChildren(const juce::ValueTree&);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiArpeggiatorAudioProcessor)
};