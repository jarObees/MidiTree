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
								 Tree::ActiveTreeManager* _activeTreeManager, 
								 InfoTabManager::InfoTabManager* _infoTabManager)
		: apvts(_apvts), 
		presetManager(_presetManager), 
		maxNumTrees(int(_apvts.getParameter("forest")->getNormalisableRange().end)), 
		activeTreeManager(_activeTreeManager),
		infoTabManager(_infoTabManager)
	{
		for (auto* tree : forestTreesUI)
			tree = nullptr;
		
		forestDataSlots.assign(maxNumTrees, { "", {} });
		///TODO: See if we can delete this later (obsolete)
		//generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		//notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		//midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
		apvts.state.addListener(this);
		bypassState = true;
		DBG("Linked genLString/notesPool to property value");
	}

	void ForestManager::configurePlantButtonForest(juce::Button* button)
	{
		plantButton = button;
		plantButton->addListener(this);
	}

	void ForestManager::configureTreeSlider(juce::Slider* slider)
	{
		forestSlider = slider;
		forestSlider->addListener(this);
	}
	void ForestManager::configureForestTrees(std::vector<jiveGui::ForestUI::TreeComponent*> trees)
	{
		forestTreesUI = trees;
		for (jiveGui::ForestUI::TreeComponent* tree : forestTreesUI)
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

	void ForestManager::configureForestResetButton(juce::Button* _resetButton)
	{
		resetButton = _resetButton;
		resetButton->addListener(this);
		jassert(resetButton != nullptr);
	}
	// Planting is the only place where we should interact with the PresetManager.
	// 
	void ForestManager::plantTree()
	{
		DBG("Attempting to plant =======================================================");

		if (!activeTreeManager->getNotesPool().isEmpty())
		{
			if (activeTreeManager->hasSavedTree())
			{
				DBG("WE PLANTING IN THIS HOE!");
				auto& dataSlot = forestDataSlots[currentForestIndex];
				dataSlot.name = activeTreeManager->getTreeName();
				dataSlot.notesPool = activeTreeManager->getNotesPool();
				
				auto& selectedTreeDisplay = forestTreesUI[currentForestIndex];
				selectedTreeDisplay->setLoadedState(true);
				selectedTreeDisplay->setActiveState(true);
				bypassButton->setToggleState(false, true); // Maybe shouldn't send notification?
			}
			else
			{
				DBG("Must plant the tree first!");
				return;
			}

		}
		else
		{
			DBG("Empty Notes Pool!");
			return;
		}
	}

	void ForestManager::sliderValueChanged(juce::Slider* slider)
	{
		// If valid, set current notes pool to the one given by the forest.
		if (forestSlider == slider)
		{
			currentForestIndex = forestSlider->getValue() - 1;
			auto currentSlottedTree = forestDataSlots[currentForestIndex];
			DBG("Current tree: " << currentSlottedTree.name);
			if (!currentSlottedTree.notesPool.isEmpty())
			{
				// Sets selected slottedTree to activeTree
				activeTreeManager->setNotesPool(currentSlottedTree.notesPool);
				activeTreeManager->setTreeName(currentSlottedTree.name);
			}

			updateTreeSlotsUI();
		}
	}

	void ForestManager::updateTreeSlotsUI()
	{
		auto* activeTree = forestTreesUI[currentForestIndex];
		DBG("Active Tree is # " << currentForestIndex);
		jassert(activeTree != nullptr);
		for (auto* tree : forestTreesUI)	// Turn off all trees
			tree->setActiveState(false);
		activeTree->setActiveState(true); // Then only turn on active tree.
	}

	void ForestManager::buttonClicked(juce::Button* button)
	{
		DBG("Forest Manager: Button Clicked!");
		if (plantButton->getComponentID() == button->getComponentID())
		{
			plantTree();
		}
		else if (resetButton->getComponentID() == button->getComponentID())
		{
			DBG("Reset button clicked");
			resetForestSlots();
		}
		else if (bypassButton->getComponentID() == button->getComponentID())
		{
			DBG("Setting bypass to" << (button->getToggleState() ? "true" : "false"));
			setBypass(button->getToggleState());
		}
	}

	void ForestManager::resetForestSlots()
	{
		DBG("Resetting all forest slots...");
		for (auto tree : forestDataSlots)
		{
			tree = Tree::MidiTree{};
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
				infoTabManager->sendDoubleLineMessage("Tree Slot " + treeSlotNum + ": " + treeName, 
													 "String: " + lString);
			}
			else
			{
				infoTabManager->sendDoubleLineMessage("Tree Slot " + treeSlotNum + ": [Empty!]",
													 "String: ---");
			}
		}
		else
			jassertfalse;
	}

	void ForestManager::mouseExit(const juce::MouseEvent& event)
	{
		DBG("Mouse exited tree component");
		DBG("Active Tree Notes Pool" << activeTreeManager->getNotesPoolAsNumString());
		infoTabManager->setActiveTreeDisplay();
	}
	void ForestManager::valueTreeRedirected(juce::ValueTree& changedTree)
	{
		///TODO: See if we can delete this later (obsolete)
		//generatedLString.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
		//notesPool.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
		//midiTreeName.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
	}
}