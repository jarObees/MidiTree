/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiArpeggiatorAudioProcessor::MidiArpeggiatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

MidiArpeggiatorAudioProcessor::~MidiArpeggiatorAudioProcessor()
{
}

//==============================================================================
const juce::String MidiArpeggiatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiArpeggiatorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MidiArpeggiatorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MidiArpeggiatorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MidiArpeggiatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiArpeggiatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MidiArpeggiatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiArpeggiatorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MidiArpeggiatorAudioProcessor::getProgramName(int index)
{
    return {};
}

void MidiArpeggiatorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{

}

//==============================================================================
void MidiArpeggiatorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    rate = static_cast<float> (sampleRate); // Get samplerate
    notes.clear();
    currentNote = 0;
    lastNoteValue = -1;
    time = 0;
    sampleNoteDivision = 1;
}

void MidiArpeggiatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiArpeggiatorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MidiArpeggiatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    genParam = apvts.getRawParameterValue("gens")->load(); // USE THIS FOR ACTUAL SHIT
    buffer.clear();
    auto* playHead = getPlayHead();
    ///TODO: Connect the notesPool here so that we can begin modifying shit based on the axiom.
    if (playHead != nullptr)
    {
        auto numSamples = buffer.getNumSamples(); // Number of samples per block.

        auto posInfoOpt = playHead->getPosition();
        
        auto ppqPosOpt = posInfoOpt->getPpqPosition();
        auto bpmOpt = posInfoOpt->getBpm();

        // Convert the ppq to samples.
        double bpm = *bpmOpt;
        double secondsPerBeat = 60.0 / bpm;
        double samplesPerBeat = rate * secondsPerBeat; // Or also samples per quarter note if it's 4/4.
        
        auto noteDuration = static_cast<int> (std::ceil(samplesPerBeat * sampleNoteDivision));
        // Checks for user midi input, to see if we add/remove notes from the set. 
        for (const auto meta : midiMessages)
        {
            const auto currentMessage = meta.getMessage();
            if (currentMessage.isNoteOn())
            {
                notes.add(currentMessage.getNoteNumber());
                DBG("ADDING NOTE...");
            }
            else if (currentMessage.isNoteOff())
            {
                DBG("REMOVING NOTE...");
                notes.removeValue(currentMessage.getNoteNumber());
            }
        }
        midiMessages.clear();
        // Working through all the notes in the notes sortedset.
        if ((time + numSamples) >= noteDuration) // If the note ends within this block. 
        {
            //
            auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1)); // Represents the number of samples from the start of the block.
            //DBG("Offset: " << offset);
            if (lastNoteValue > 0) // If there has been a previous note played.
            {
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset); // Turn note off.
                DBG("Turning off" << lastNoteValue << " at offset: " << offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0) // If there's still notes to be played...
            {
                currentNote = (currentNote + 1) % notes.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.   
                lastNoteValue = notes[currentNote]; // Gets the note...
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset); // Plays the note.
                DBG("Turning note on: " << lastNoteValue << " at offset: " << offset);
            }
        }
        time = (time + numSamples) % noteDuration;
    }
}

//==============================================================================
bool MidiArpeggiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiArpeggiatorAudioProcessor::createEditor()
{
    return new MidiArpeggiatorAudioProcessorEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MidiArpeggiatorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData, true); // TODO: Should this be true or not?
    apvts.state.writeToStream(mos);
    userRulesetNode.writeToStream(mos);
    userAxiomNode.writeToStream(mos);
}

void MidiArpeggiatorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //WARNING: This is NOT thread safe. BUT. If it ain't broke...
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
    {
        apvts.replaceState(tree);
        userRulesetNode.copyPropertiesAndChildrenFrom(tree, nullptr);
        userAxiomNode.copyPropertiesAndChildrenFrom(tree, nullptr);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout 
    MidiArpeggiatorAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    // Global Generation Variables
    params.add(std::make_unique<juce::AudioParameterInt>("gens", "Generations", 1, 10, 1));

    return params;
 }
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiArpeggiatorAudioProcessor();
}