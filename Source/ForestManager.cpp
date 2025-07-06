#include <JuceHeader.h>
#include "ForestManager.h"
#include "PresetManager.h"
#include "Ids.h"

namespace Forest
{
	ForestManager::ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, juce::Array<int>& _currentNotesPool)
		: apvts(_apvts), presetManager(_presetManager), maxNumTrees(int(_apvts.getParameter("forest")->getNormalisableRange().end)), currentNotesPool(_currentNotesPool)
	{
		for (auto* tree : forestTrees)
			tree = nullptr;

		forestDataSlots.assign(maxNumTrees, { "", {} });
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
		apvts.state.addListener(this);
		bypass = true;
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
	void ForestManager::configureForestTrees(std::vector<juce::Component*> trees)
	{
		for (auto* tree : trees)
			jassert(tree != nullptr);
		forestTrees = trees;
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