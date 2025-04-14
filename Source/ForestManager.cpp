#include <JuceHeader.h>
#include "ForestManager.h"
#include "PresetManager.h"
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
	ForestManager::ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, juce::Array<int>& _currentNotesPool)
		: apvts(_apvts), presetManager(_presetManager), maxNumTrees(int(apvts.getParameter("forest")->getNormalisableRange().end)), currentNotesPool(_currentNotesPool)
	{
		forestDataSlots.assign(maxNumTrees, { "", {} });
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
		apvts.state.addListener(this);
		DBG("Linked genLString/notesPool to property value");
	}

	void ForestManager::configureForestButton(juce::Button* button)
	{
		forestButton = button;
		forestButton->addListener(this);
	}

	void ForestManager::configureTreeSlider(juce::Slider* slider)
	{
		forestSlider = slider;
		forestSlider->setValue(1); // Sets default value.
		forestSlider->addListener(this);
	}

	// Planting is the only place where we should interact with the PresetManager.
	void ForestManager::plantTree()
	{
		DBG("Attempting to plant =======================================================");

		if (!generatedLString.getValue().isVoid()) // Guarantees that midiTree is valid and has been saved.
		{
			if (!presetManager.isModified)
			{
				DBG("WE PLANTING IN THIS HOE!");
				auto& dataSlot = forestDataSlots[currentForestIndex];
				dataSlot.first = midiTreeName.getValue();
				dataSlot.second = currentNotesPool;
			}
			else
			{
				DBG("Preset manager is currently modified!");
				return;
			}

		}
		else
		{
			DBG("generatedLString is void!");
			return;
		}
	}

	void ForestManager::sliderValueChanged(juce::Slider* slider)
	{
		// If valid, set current notes pool to the one given by the forest.
		// TODO: Fix thing going out of range.
		if (forestSlider == slider)
		{
			currentForestIndex = forestSlider->getValue() - 1;
			DBG("Current tree: " << forestDataSlots[currentForestIndex].first);
			auto notesPool = forestDataSlots[currentForestIndex].second;
			if (!notesPool.isEmpty())
			{
				currentNotesPool = notesPool;
			}
		}
	}
	void ForestManager::buttonClicked(juce::Button* button)
	{
		if (forestButton = button)
		{
			plantTree();
		}
	}
	void ForestManager::valueTreeRedirected(juce::ValueTree& changedTree)
	{
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
	}
}