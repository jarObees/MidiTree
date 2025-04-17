/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Ids.h"
#include <chrono>


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
    DBG("Finished Constructing MAIN Audio Processor");
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

void MidiArpeggiatorAudioProcessor::getAutomatableParams()
{
    velParam = apvts.getRawParameterValue("vel")->load();

    // Updates note Rate Param ==========================================================
    auto start = std::chrono::high_resolution_clock::now();
    const auto noteRateIndex = apvts.getRawParameterValue("noteRate")->load();
    const std::string& noteRateKey = noteRateKeys[noteRateIndex];
    auto umap_it = noteRateMap.find(noteRateKey);
    if (umap_it != noteRateMap.end())
    {
        noteRate = umap_it->second; // Updates the param here.
    }
    else
    {
        DBG("COULD NOT FIND" << noteRateKey);
        jassertfalse;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    // DBG("Note Rate Finder Time: " << duration.count() << " ms");
    // Updates note Type =================================================================
    noteType = apvts.getRawParameterValue("noteType")->load();

}

void MidiArpeggiatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    if (currentNotesPool.isEmpty())
    {
        DBG("Notes pool empty, bypassing...");
        midiMessages.clear();
        return;
    }

	DBG("Processing Block ==============");
    getAutomatableParams();
    
    auto* playHead = getPlayHead();
    const int samplesPerBlock = buffer.getNumSamples(); // Number of samples per block

    // Sample Position
    auto posInfoOpt = playHead->getPosition();
    auto bpm = *posInfoOpt->getBpm();
    const double secondsPerQuarterNote = 60.0 / bpm;
    const double secondsPerDottedNote = secondsPerQuarterNote + (secondsPerQuarterNote / 2);
    const double samplesPerQuarter = sampRate * secondsPerQuarterNote;
    const double samplesPerDotted = sampRate * secondsPerDottedNote;

    const int samplesPerNote = (int(noteType) == 0) ? static_cast<int> (std::ceil(samplesPerQuarter * noteRate))
                                                    : static_cast<int> (std::ceil(samplesPerDotted * noteRate));
    // Checks for user midi input.
    int midiLocalPos = 0;
    bool ismidiTurnedOffInThisBlock = false;
    for (const auto meta : midiMessages)
    {
        // Sets midiAxiom to note that was played.
        const auto currentMessage = meta.getMessage();
        if (currentMessage.isNoteOn())
        {
			currentNote = 0; // Resets the note to first note in notePool, so that each midi triggers a new stream of lsys notes.
            midiAxiom = currentMessage.getNoteNumber();
			midiLocalPos = currentMessage.getTimeStamp();
			isFirstNote = true;
			isMidiHeldDown = true;
            DBG("Midi Note On Message Received at " << currentMessage.getTimeStamp());
        }

        else if (currentMessage.isNoteOff())
        {
			isMidiHeldDown = false;
            mustTurnOff = true;
            DBG("midiNoteToPlay= " << midiNoteToPlay);
            DBG("isMiiHeldDown = FALSE");
            DBG("Midi Note Off  Message Received." << currentMessage.getTimeStamp());
        }
    }
    midiMessages.clear();

    // Handles the quantization for the first note and triggers a timer for all future notes.
    if (isFirstNote)
    {
        DBG("Is first note...");
        // If the playhead is running...
        if (posInfoOpt.hasValue() && posInfoOpt->getIsPlaying())
        {
            DBG("Playhead Running...");
            // Quantize note =====================
            // Grab Global Position.
            const int blockSamplePos = static_cast<int>(*posInfoOpt->getTimeInSamples());
            const int globalSamplePos = blockSamplePos + midiLocalPos;
            DBG("FN Block Sample Position: " << blockSamplePos);
            DBG("FN Global Sample Position: " << globalSamplePos);

            // Find nearest future quantized sample pos.
            //TODO: NEXT QUANTIZED NOTE IS NOT CORRECT.
            // For some reason, the first is played twice.
            // Maybe lets restrict the first note to only be played at the next sample position. aka don't allow it to be played at the very beginning. 
            const int samplesIntoQuantizedNote = globalSamplePos % samplesPerNote; // If 0, then we are at the quantized note.
            const int samplesToNextQuantizedNote = (samplesIntoQuantizedNote == 0) ? 0 : (samplesPerNote - samplesIntoQuantizedNote);

            midiNoteToPlay = currentNotesPool[currentNote] + midiAxiom;
            DBG("FN*** Turning note on: " << midiNoteToPlay);
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), samplesToNextQuantizedNote); // Plays the note.
            currentNote = (currentNote + 1) % currentNotesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
            DBG("FN Last note value after note on = " << midiNoteToPlay);
            timer = 0;
        }
        // Else if the user hasn't started the playhead (say in a live performance).
        else
        {
            DBG("Playhead not active, live performance detected...");
            midiNoteToPlay = currentNotesPool[currentNote] + midiAxiom;
            DBG("FN *** Turning note on: " << midiNoteToPlay << " at sample: " << midiLocalPos);
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), midiLocalPos); // Plays the note.
            currentNote = (currentNote + 1) % currentNotesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
            DBG("FN Last note value after note on = " << midiNoteToPlay);
            timer = 0;
        }
        timer += samplesPerBlock;
    }

    // Handles all note arpeggiation besides the first note based on the timer.
	if (isMidiHeldDown == true && isFirstNote == false || mustTurnOff)
	{
        // Find nearest future quantized sample pos.
        const int samplesIntoQuantizedNote = timer % samplesPerNote; // If 0, then we are at the quantized note.
        const int samplesToNextQuantizedNote = (samplesIntoQuantizedNote == 0) ? 0 : (samplesPerNote - samplesIntoQuantizedNote);
        
        // Working through all the notes in the notePool.
        if (samplesToNextQuantizedNote < samplesPerBlock || samplesIntoQuantizedNote == 0) // If a quantized note should be turned on/off within this block...
        {
            // We will always turn a note off...
            DBG("Last note value is currently: " << midiNoteToPlay);
            DBG("*** Turning off" << midiNoteToPlay << " at sample: " << samplesToNextQuantizedNote);
            midiMessages.addEvent(juce::MidiMessage::noteOff(1, midiNoteToPlay), samplesToNextQuantizedNote); // Turn note off.

            if (!mustTurnOff) // If we need to play a note...
            {
                midiNoteToPlay = currentNotesPool[currentNote] + midiAxiom;
                DBG("*** Turning note on: " << midiNoteToPlay << " at sample: " << samplesToNextQuantizedNote);
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, midiNoteToPlay, (juce::uint8)127), samplesToNextQuantizedNote); // Plays the note.
                currentNote = (currentNote + 1) % currentNotesPool.size(); // Goes to the next note, and loops over to the start once we get to end of sequence.
                DBG("Last note value after note on = " << midiNoteToPlay);
            }
            else
            {
                mustTurnOff = false;
            }
        }
		timer += samplesPerBlock;
	}
	isFirstNote = false;
    DBG("End -----");
}

//==============================================================================
bool MidiArpeggiatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

// Returns a map of images to be used in the editor.
std::unordered_map<std::string, juce::Image> MidiArpeggiatorAudioProcessor::getImages()
{
    std::unordered_map<std::string, juce::Image> imageCollection;
	juce::Image knobTestWhiteFilmstrip = juce::ImageFileFormat::loadFrom(BinaryData::knobTestWhite_png, 
                                                                         BinaryData::knobTestWhite_pngSize);
    juce::Image horiSliderWhiteFilmstrip = juce::ImageFileFormat::loadFrom(BinaryData::_128_HORISLIDER_FILMSTRIP_png, 
                                                                           BinaryData::_128_HORISLIDER_FILMSTRIP_pngSize);
	imageCollection[jive_gui::ImageIds::mainKnobFilmstrip] = knobTestWhiteFilmstrip;
	imageCollection[jive_gui::ImageIds::horiKnobFilmstrip] = horiSliderWhiteFilmstrip;
	return imageCollection;
}
juce::AudioProcessorEditor* MidiArpeggiatorAudioProcessor::createEditor()
{
    // 331 ms
    // return new MidiArpeggiatorAudioProcessorEditor(*this);
	DBG("------ CREATING EDITOR ------");
    jassert(JIVE_IS_PLUGIN_PROJECT);
    // SETUP some basic stuff.
	imageCollection = getImages(); // Images have to be retrieved during runtime not compile time.

    view = jive_gui::getView(imageCollection);

    if (auto editor = viewInterpreter.interpret(view, this))
    {
        if (dynamic_cast<juce::AudioProcessorEditor*>(editor.get()))
        {
            
            viewInterpreter.listenTo(*editor);
            
			// Attaches/Configures automatable parameters. ========================================================
            if (auto* comboBoxTingy = dynamic_cast<juce::ComboBox*>(jive::findItemWithID(*editor, jive_gui::StringIds::noteTypeComboBox)
                                                                    ->getComponent().get()))
            {
                comboBoxTingy->addItem(comboBoxNoteTypes[0], 1);
                comboBoxTingy->addItem(comboBoxNoteTypes[1], 2);
                comboBoxTingy->setSelectedItemIndex(1);
            }
            else
                jassertfalse;
            jive::findItemWithID(*editor, jive_gui::StringIds::noteTypeComboBox)
                ->attachToParameter(apvts.getParameter("noteType"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::noteRateKnob)
                ->attachToParameter(apvts.getParameter("noteRate"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::noteRateKnobText)
                ->attachToParameter(apvts.getParameter("noteRate"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::midiVelocityKnob)
                ->attachToParameter(apvts.getParameter("vel"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::midiVelocityKnobText)
                ->attachToParameter(apvts.getParameter("vel"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::forestSlider)
                ->attachToParameter(apvts.getParameter("forest"), &undoManager);
            jive::findItemWithID(*editor, jive_gui::StringIds::generationsKnob)
                ->attachToParameter(apvts.getParameter("gens"), &undoManager);
            
            
            // Links and sets up non-automatable parameters. ========================================================
            if (auto* saveButtonTingy = dynamic_cast<juce::Button*>(jive::findItemWithID(*editor, jive_gui::StringIds::saveButton)
                 ->getComponent().get()))
            {
                presetManager.configureSaveButtonComponent(saveButtonTingy);

            }
            else
                jassertfalse;
            if (auto* presetComboBox = dynamic_cast<juce::ComboBox*>
                (jive::findItemWithID(*editor, jive_gui::StringIds::presetComboBox)
                 ->getComponent().get()))
            {
                presetManager.configureComboBoxComponent(presetComboBox);
            }
            else
                jassertfalse;
            
            // Ruleset Textbox ======================================================================================
            if (auto* textEditorTingy = dynamic_cast<juce::TextEditor*>
                (jive::findItemWithID(*editor, jive_gui::StringIds::rulesetTextbox)
                 ->getComponent().get()))
            {
				lSystemManager.configureRulesetInputTextEditor(textEditorTingy);
            }
            else
                jassertfalse;
            // Axiom Textbox. =============================================
            if (auto* axiomEditor = dynamic_cast<juce::TextEditor*>
                (jive::findItemWithID(*editor, jive_gui::StringIds::axiomTextBox)
                 ->getComponent().get()))
            {
                lSystemManager.configureAxiomInputTextEditor(axiomEditor);
            }
            else
                jassertfalse;
			// Grow Button =========================================================================================
			if (auto* growButton = dynamic_cast<juce::Button*>
				(jive::findItemWithID(*editor, jive_gui::StringIds::growButton)
				 ->getComponent().get()))
			{
				lSystemManager.configureGrowButton(growButton);
			}
			else
				jassertfalse;
			if (auto* gensKnob = dynamic_cast<juce::Slider*>
				(jive::findItemWithID(*editor, jive_gui::StringIds::generationsKnob)
				 ->getComponent().get()))
			{
				lSystemManager.configureGensKnob(gensKnob);
			}
			else
				jassertfalse;
            // Forest Manager ========================================================================================
            if (auto* forestSlider = dynamic_cast<juce::Slider*>
                (jive::findItemWithID(*editor, jive_gui::StringIds::forestSlider)
                 ->getComponent().get()))
            {
                forestManager.configureTreeSlider(forestSlider);
            }
            else
                jassertfalse;
            if (auto* forestButton = dynamic_cast<juce::Button*>
                (jive::findItemWithID(*editor, jive_gui::StringIds::plantButton)
                 ->getComponent().get()))
            {
                forestManager.configureForestButton(forestButton);
            }
            else
                jassertfalse;

            //jive::findItemWithID(*editor, "midiVelocity-label")->attachToParameter(apvts.getParameter("vel"), &undoManager);
            return dynamic_cast<juce::AudioProcessorEditor*>(editor.release());
        }
    }
    DBG("RULESET AFTER EDITOR IS CREATED:" << (apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr)));
    // return dynamic_cast<juce::AudioProcessorEditor*>(viewInterpreter.interpret(view, this).release());
}

//==============================================================================
void MidiArpeggiatorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    DBG("------ GETTING STATE INFORMATION ------");
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData, true); // TODO: Should this be true or not?
    apvts.state.writeToStream(mos);
}

//TODO: Figure out why this shit aint working.
void MidiArpeggiatorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    DBG("------ SETTING STATE INFORMATION ------");

    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout 
MidiArpeggiatorAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    // Global Generation Variables
    params.add(std::make_unique<juce::AudioParameterInt>("gens", "Generations", 1, 10, 5)); // IGNORE DEFAULT PARAMETER. This is set in appropriate manager class.
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
    params.add(std::make_unique<juce::AudioParameterInt>("forest", "Forest Selection", 1, 7, 2)); // IGNORE DEFAULT PARAMETER. This is set in appropriate manager class.

    // Takes each note rate, "1/4", "1/18", etc. and creates an array for the param display.
    for (const auto& pair : noteRateMap)
    {
        std::string thingy = pair.first;
        noteRateKeys.push_back(pair.first);
    }
    
    // Converts the noteRateKeys to a juce::stringArray.
    juce::StringArray _noteRateKeys;
    for (std::string element : noteRateKeys)
    {
        _noteRateKeys.add(element);
    }
    params.add(std::make_unique<juce::AudioParameterChoice>("noteRate", "Rate", _noteRateKeys, 5)); //TODO: Fix the shit not showing up.
    return params;
 }
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiArpeggiatorAudioProcessor();
}