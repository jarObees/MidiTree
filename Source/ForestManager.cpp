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

		forestDataSlots.assign(maxNumTrees, { "", {} });
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
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
		forestSlider->addListener(this);
	}

	void ForestManager::plantTree()
	{
		DBG("Attempting to plant =======================================================");

		if (!generatedLString.getValue().isVoid())
		{
			if (true) //TODO: FIX NTOE POOLS BUGGING OUT.
			{
				auto& dataSlot = forestDataSlots[currentForestIndex];
				dataSlot.first = generatedLString.getValue();
				
				juce::Array<int> extractedArray;
				for (const auto& note : *notesPool.getValue().getArray())
				{
					DBG("Added " << (int)note << "to notes pool.");
					extractedArray.add((int)note);
				}
				DBG("Successfully added notes to notesPool!");
			}
			else
			{
				DBG(apvtsPropIds::notesPoolVectorStringProperty << "IS VOID");
				return;
			}
		}
		else
		{
			DBG(apvtsPropIds::generatedLsysStringProperty << " IS VOID");
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
	void ForestManager::valueTreeRedirected(juce::ValueTree& changedTree)
	{
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
	}
}