#include <JuceHeader.h>
#include "ForestManager.h"
#include "Ids.h"

//TODO: 
// Create class that handles the hori slider.
/*

Planting a tree======================================
To "Plant" a tree, we need a valid preset.
	A valid preset is one which has a generatedLString.
	If the current preset has a generatedLString, then we can plant it.
		else do nothing, and maybe throw a message like (you have to grow your tree first!)

Using the forest ======================================
	Each container
		Has generatedLString (user legible)
		Has notesPool (midi stuff)
	If it's a non-empty container
		Display the generatedLString in the ui
		notesPool in process block will use this selected notesPool.
	else
		bypass processblock
Containers ============================================
	Selected through the horislider value.
	Perhaps an index into some vector with some data structure.

Forest Manager?
	Similar to presetManager but you can save/load in your forests.
The container simply has the generatedLString and the notesPool. 
*/
namespace Forest
{
	ForestManager::ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager)
		: apvts(_apvts), presetManager(_presetManager), maxNumTrees(int(apvts.getParameter("forest")->getNormalisableRange().end))
	{
		// Sets up each data slot with some dummy values.
		forestDataSlots.reserve(maxNumTrees);
		for (auto& lStringNotesPoolPair : forestDataSlots)
		{
			juce::Array<juce::var> emptyVector;
			lStringNotesPoolPair = std::make_pair("", emptyVector);
		}
	}

	void ForestManager::configureForestButton(juce::Button* button)
	{
		forestButton = button;
		forestButton->addListener(this);
	}

	void ForestManager::configureTreeSlider(juce::Slider* slider)
	{
		forestSlider = slider;
		forestSlider->addListener(this);
	}

	void ForestManager::plantTree()
	{
		DBG("Attempting to plant");
		if (!apvts.state.getProperty(apvtsPropIds::generatedLsysStringProperty).isVoid() &&
			!apvts.state.getProperty(apvtsPropIds::notesPoolVectorStringProperty).isVoid())
		{
			auto& dataSlot = forestDataSlots[currentForestIndex];
			dataSlot.first = apvts.state.getProperty(apvtsPropIds::generatedLsysStringProperty);
			auto* thing = apvts.state.getProperty(apvtsPropIds::notesPoolVectorStringProperty).getArray();
			
			jassert(thing != nullptr);
		}

		else
		{
			DBG(apvtsPropIds::generatedLsysStringProperty << " property not found");
			return;
		}
	}

	void ForestManager::sliderValueChanged(juce::Slider* slider)
	{
		if (forestSlider == slider)
		{
			currentForestIndex = forestSlider->getValue();
		}
	}
	void ForestManager::buttonClicked(juce::Button* button)
	{
		if (forestButton = button)
		{
			plantTree();
		}
	}
}