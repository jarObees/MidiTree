#include <JuceHeader.h>
#include "ForestManager.h"
#include "PresetManager.h"
#include "InfoTabManager.h"
#include "Tree.h"
#include "Ids.h"

namespace Forest
{
	ForestManager::ForestManager(juce::AudioProcessorValueTreeState& _apvts, 
								 Preset::PresetManager& _presetManager, 
								 Tree::MidiTree* _activeMidiTree, 
								 InfoTabManager::InfoTabManager* _infoTabManager)
		: apvts(_apvts), 
		presetManager(_presetManager), 
		maxNumTrees(int(_apvts.getParameter("forest")->getNormalisableRange().end)), 
		activeMidiTree(_activeMidiTree),
		infoTabManager(_infoTabManager)
	{
		for (auto* tree : forestTrees)
			tree = nullptr;
		
		forestDataSlots.assign(maxNumTrees, { "", {} });
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
		apvts.state.addListener(this);
		bypassState = true;
		DBG("Linked genLString/notesPool to property value");
	}

	void ForestManager::configurePlantButtonForest(juce::Button* button)
	{
		forestButton = button;
		forestButton->addListener(this);
	}

	void ForestManager::configureTreeSlider(juce::Slider* slider)
	{
		forestSlider = slider;
		forestSlider->addListener(this);
	}
	void ForestManager::configureForestTrees(std::vector<jiveGui::ForestUI::TreeComponent*> trees)
	{
		forestTrees = trees;
		for (jiveGui::ForestUI::TreeComponent* tree : forestTrees)
		{
			tree->addMouseListener(this, true);
		}
	}

	void ForestManager::configureForestBypassButton(juce::Button* _bypassButton)
	{
		bypassButton = _bypassButton;
		jassert(bypassButton != nullptr);
	}

	bool ForestManager::isBypass() const
	{
		return bypassState;
	}

	void ForestManager::setBypass(bool newState)
	{
		bypassState = newState;
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
				dataSlot.name = midiTreeName.getValue();
				dataSlot.notesPool = activeMidiTree->notesPool;
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
			DBG("Current tree: " << forestDataSlots[currentForestIndex].name);
			auto notesPool = forestDataSlots[currentForestIndex].notesPool;
			if (!notesPool.isEmpty())
			{
				activeMidiTree->notesPool = notesPool;
			}
			// Update UI
			auto* activeTree = forestTrees[currentForestIndex];
			DBG("Active Tree is # " << currentForestIndex);
			jassert(activeTree != nullptr);
			for (auto* tree : forestTrees)	// Turn off all trees
				tree->setActiveState(false);
			activeTree->setActiveState(true); // Then only turn on active tree.
		}
	}
	void ForestManager::buttonClicked(juce::Button* button)
	{
		if (forestButton = button)
		{
			plantTree();
		}
	}

	// Used for sending messages to InfoTabManager
	void ForestManager::mouseEnter(const juce::MouseEvent& event)
	{
		DBG("Mouse entered tree component");
		if (auto* tree = event.eventComponent->findParentComponentOfClass<jiveGui::ForestUI::TreeComponent>())
		{
			juce::String treeSlotNum = juce::String(tree->idNum);
			auto treeData = forestDataSlots[tree->idNum];

			if (tree->getLoadedState())
			{
				juce::String treeName = treeData.name;
				std::stringstream ss;
				for (int num : treeData.notesPool)
					ss << num;
				juce::String lString = ss.str();
				infoTabManager->setDoubleLineMessage("Tree Slot " + treeSlotNum + ": " + treeName, 
													 "String: " + lString);
			}
			else
			{
				infoTabManager->setDoubleLineMessage("Tree Slot " + treeSlotNum + ": [Empty!]",
													 "String: ---");
			}
		}
		else
			jassertfalse;
	}

	void ForestManager::mouseExit(const juce::MouseEvent& event)
	{
		DBG("Mouse exited tree component");
		infoTabManager->setDefaultState();
	}
	void ForestManager::valueTreeRedirected(juce::ValueTree& changedTree)
	{
		generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
	}
}