/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
// Testing the thing out so that it actually copies it over.
#include <JuceHeader.h>

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

    float sampleNoteDivision;
    static juce::AudioProcessorValueTreeState::ParameterLayout
        createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout()};
    
    // Defining Value Trees for non numerical parameters.
    static const inline juce::Identifier userRulesetNodeType{"userRulesetNode"};
    juce::ValueTree userRulesetNode{ userRulesetNodeType };

    static const inline juce::Identifier userAxiomNodeType{ "userAxiomNode" };
    juce::ValueTree userAxiomNode{ userAxiomNodeType };

private:
    float rate;
    int currentNote, lastNoteValue; // Indexes into notes sorted set.
    int time; // Representts the time that has elapsed since the start of the note on output. 

    juce::SortedSet<int> notes; 
    // LSystemProcessor lsysproc
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiArpeggiatorAudioProcessor)
};