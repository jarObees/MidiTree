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
    sampRate = static_cast<float> (sampleRate); // Get samplerate
    currentNote = 0;
    midiNoteToPlay = -1;
    // midiAxiom = -1; // Default state is set to -1.

    isFirstNote = false;
    isMidiHeldDown = false;
    mustTurnOff = false;
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


// Gets the parameters from the AudioProcessorValueTreeState to use in the processBlock. 
void MidiArpeggiatorAudioProcessor::getParams()
{
    genParam = apvts.getRawParameterValue("gens")->load();
    velParam = apvts.getRawParameterValue("vel")->load();

    // Converts the rawParamValue to to the correct float one.
    const auto noteRateIndex = apvts.getRawParameterValue("noteRate")->load();
    const std::string& noteRateKey = lsysProcessor.noteRateKeys[noteRateIndex];
    auto umap_it = lsysProcessor.noteRateMap.find(noteRateKey);
    if (umap_it != lsysProcessor.noteRateMap.end())
    {
        noteRate = umap_it->second; // Updates the param here.
    }
    else
    {
        DBG("COULD NOT FIND" << noteRateKey);
    }
}

// FOR NOW, WE WILL ASSUME THAT WE ARE NOT GOING TO CHANGE THE L SYSTEM DURING PLAY.
void MidiArpeggiatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	DBG("Processing Block==============");
    getParams();
    buffer.clear();
    
 //   auto* playHead = getPlayHead();
 //   const int samplesPerBlock = buffer.getNumSamples(); // Number of samples per block

 //   // Sample Position
 //   auto posInfoOpt = playHead->getPosition();
 //   auto bpm = *posInfoOpt->getBpm();
 //   const double secondsPerQuarterNote = 60.0 / bpm;
 //   const double samplesPerQuarter = sampRate * secondsPerQuarterNote;
 //   const int samplesPerNote = static_cast<int> (std::ceil(samplesPerQuarter * noteRate));
 //       
 //   // Checks for user midi input.
 //   int midiLocalPos = 0;
 //   bool ismidiTurnedOffInThisBlock = false;
 //   for (const auto meta : midiMessages)
 //   {
 //       // Sets midiAxiom to note that was played.
 //       const auto currentMessage = meta.getMessage();
 //       if (currentMessage.isNoteOn())
 //       {
	//		currentNote = 0; // Resets the note to first note in notePool, so that each midi triggers a new stream of lsys notes.
 //           midiAxiom = currentMessage.getNoteNumber();
	//		midiLocalPos = currentMessage.getTimeStamp();
	//		isFirstNote = true;
	//		isMidiHeldDown = true;
 //           DBG("Midi Note On Received");
 //       }
 //       /// TODO: Currently, since isMidiHeldDown is set to false, the main timer logic is skipped over, and as such the final note is not turned off.
 //       /// We need to make sure that once we get a note off within the block, we 'cleanup' as in turn off any remaining notes that might be playing.
 //       else if (currentMessage.isNoteOff())
 //       {
	//		isMidiHeldDown = false;
 //           mustTurnOff = true;
 //           DBG("midiNoteToPlay= " << midiNoteToPlay);
 //           DBG("isMiiHeldDown = FALSE");
 //           DBG("Midi Note Off Received.");
 //       }
 //   }
 //   midiMessages.clear();

 //   // Handles the quantization for the first note and triggers a timer for all future notes.
 //   if (isFirstNote)
 //   {
 //       // If the playhead is running...
 //       if (posInfoOpt.hasValue() && posInfoOpt->getIsPlaying())
 //       {
 //           // Quantize note =====================
 //           // Grab Global Position.
 //           const int blockSamplePos = static_cast<int>(*posInfoOpt->getTimeInSamples());
 //           const int globalSamplePos = blockSamplePos + midiLocalPos;
 //           DBG("FN Block Sample Position: " << blockSamplePos);
 //           DBG("FN Global Sample Position: " << globalSamplePos);

 //           // Find nearest future quantized sample pos.
 //           const int samplesIntoQuantizedNote = globalSamplePos % samplesPerNote; // If 0, then we are at the quantized note.
 //           const int samplesToNextQuantizedNote = (samplesIntoQuantizedNote == 0) ? 0 : (samplesPerNote - samplesIntoQuantizedNote);

 //           midiNoteToPlay = lsysProcessor.notesPool[currentNote] + midiAxiom;
 //           DBG("FN*** Turning note on: " << midiNoteToPlay);
 //           midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), samplesToNextQuantizedNote); // Plays the note.
 //           currentNote = (currentNote + 1) % lsysProcessor.notesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
 //           DBG("FN Last note value after note on = " << midiNoteToPlay);
 //           timer = 0;
 //       }
 //       // Else if the user hasn't started the playhead (say in a live performance).
 //       else
 //       {
 //           midiNoteToPlay = lsysProcessor.notesPool[currentNote] + midiAxiom;
 //           DBG("FN *** Turning note on: " << midiNoteToPlay << " at sample: " << midiLocalPos);
 //           midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), midiLocalPos); // Plays the note.
 //           currentNote = (currentNote + 1) % lsysProcessor.notesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
 //           DBG("FN Last note value after note on = " << midiNoteToPlay);
 //           timer = 0;
 //       }
 //       timer += samplesPerBlock;
 //   }

 //   // Handles all note arpeggiation besides the first note based on the timer.
	//if (isMidiHeldDown == true && isFirstNote == false || mustTurnOff)
	//{
 //       // Find nearest future quantized sample pos.
 //       const int samplesIntoQuantizedNote = timer % samplesPerNote; // If 0, then we are at the quantized note.
 //       const int samplesToNextQuantizedNote = (samplesIntoQuantizedNote == 0) ? 0 : (samplesPerNote - samplesIntoQuantizedNote);
 //       
 //       // Working through all the notes in the notePool.
 //       if (samplesToNextQuantizedNote < samplesPerBlock || samplesIntoQuantizedNote == 0) // If a quantized note should be turned on/off within this block...
 //       {
 //           // We will always turn a note off...
 //           DBG("Last note value is currently: " << midiNoteToPlay);
 //           DBG("*** Turning off" << midiNoteToPlay << " at sample: " << samplesToNextQuantizedNote);
 //           midiMessages.addEvent(juce::MidiMessage::noteOff(1, midiNoteToPlay), samplesToNextQuantizedNote); // Turn note off.

 //           if (mustTurnOff != true) // If we need to play a note...
 //           {
 //               midiNoteToPlay = lsysProcessor.notesPool[currentNote] + midiAxiom;
 //               DBG("*** Turning note on: " << midiNoteToPlay << " at sample: " << samplesToNextQuantizedNote);
 //               midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), samplesToNextQuantizedNote); // Plays the note.
 //               currentNote = (currentNote + 1) % lsysProcessor.notesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
 //               DBG("Last note value after note on = " << midiNoteToPlay);
 //           }
 //           else
 //           {
 //               mustTurnOff = false;
 //           }
 //       }
	//	timer += samplesPerBlock;
	//}
	//isFirstNote = false;
}

//==============================================================================
bool MidiArpeggiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiArpeggiatorAudioProcessor::createEditor()
{
    jassert(JIVE_IS_PLUGIN_PROJECT);
    view = jiveUI::getView();
    if (auto editor = viewInterpreter.interpret(view, this))
    {
        if (dynamic_cast<juce::AudioProcessorEditor*>(editor.get()))
        {
            viewInterpreter.listenTo(*editor);
            //TODO: FIND THE PARAMETERS HERE
            jive::findItemWithID(*editor, "noteRate-knob")->attachToParameter(apvts.getParameter("noteRate"), &undoManager);
            jive::findItemWithID(*editor, "midiVelocity-knob")->attachToParameter(apvts.getParameter("vel"), &undoManager);
            jive::findItemWithID(*editor, "forest-slider")->attachToParameter(apvts.getParameter("forest"), &undoManager);
            //jive::findItemWithID(*editor, "midiVelocity-label")->attachToParameter(apvts.getParameter("vel"), &undoManager);
            return dynamic_cast<juce::AudioProcessorEditor*>(editor.release());
        }
    }
    // return dynamic_cast<juce::AudioProcessorEditor*>(viewInterpreter.interpret(view, this).release());
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
    params.add(std::make_unique<juce::AudioParameterInt>("vel", "Midi Velocity", 1, 127, 100,
                                                         juce::AudioParameterIntAttributes{}
                                                         .withStringFromValueFunction(
                                                             [](int value, int)
                                                             {
                                                                 return std::to_string(value);
                                                             })
                                                         .withValueFromStringFunction([](const juce::String& text)
                                                                                      {
                                                                                          return text.getIntValue();
                                                                                      })
                                                         .withAutomatable(true)));
    params.add(std::make_unique<juce::AudioParameterChoice>("noteType", "Note Type", comboBoxNoteTypes, 0));
    params.add(std::make_unique<juce::AudioParameterInt>("forest", "Forest Selection", 1, 7, 1));

    // Takes each note rate, "1/4", "1/18", etc. and creates an array for the param display.
    for (const auto& pair : LSystemProcessor::noteRateMap)
    {
        std::string thingy = pair.first;
        lsysProcessor.noteRateKeys.push_back(pair.first);
    }
    
    // Converts the noteRateKeys to a juce::stringArray.
    juce::StringArray _noteRateKeys;
    for (std::string element : lsysProcessor.noteRateKeys)
    {
        _noteRateKeys.add(element);
    }
    params.add(std::make_unique<juce::AudioParameterChoice>("noteRate", "Rate", _noteRateKeys, 5));
    return params;
 }
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiArpeggiatorAudioProcessor();
}