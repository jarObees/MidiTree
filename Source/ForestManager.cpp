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
		treeContainer.reserve(maxNumTrees);

		for (std::pair<juce::String, juce::String>& midiTreeLStrings : treeContainer)
		{
			midiTreeLStrings = std::make_pair("", "");
		}
	}

	void ForestManager::configureTreeSlider(juce::Slider* slider)
	{
		forestSlider = slider;
		forestSlider->addListener(this);
	}

	void ForestManager::plantTree()
	{
		// Inserts a pair of string data points which tell us stuff. Good stuff.
		if (!apvts.state.getProperty(apvtsPropIds::generatedLsysStringProperty).isVoid() &&
			!apvts.state.getProperty(apvtsPropIds::notesPoolProperty).isVoid())
		{
			std::pair<juce::String, juce::String>& midiTreeLStrings = treeContainer[currentForestIndex];
			midiTreeLStrings.first = apvts.state.getProperty(apvtsPropIds::generatedLsysStringProperty);
			midiTreeLStrings.second = apvts.state.getProperty(apvtsPropIds::notesPoolProperty);
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
}