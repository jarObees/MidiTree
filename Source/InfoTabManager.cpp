#include "InfoTabManager.h"
namespace InfoTabManager
{
	InfoTabManager::InfoTabManager(Tree::ActiveTreeManager* _activeTreeManager)
	{
		activeTreeManager = _activeTreeManager;
		activeTreeManager->addListener(this);
	}

	void InfoTabManager::onActiveTreeChanged(std::shared_ptr <Tree::MidiTree>)
	{
		DBG("Active Tree Changed!");
		updateActiveTreeShower();
	}

	void InfoTabManager::updateActiveTreeShower()
	{
		if (!activeTreeManager->getNotesPool().isEmpty())
		{
			DBG("Setting tree shower to ACTIVE");
			treeShower->setToggleState(true, true);
		}
		else
		{
			DBG("Setting tree shower to INACTIVE");
			treeShower->setToggleState(false, true);
		}
	}

	void InfoTabManager::connectInfoTab(jive::GuiItem* item)
	{
		textItem = item;
		setDefaultState();
	}
	void InfoTabManager::connectActiveTreeShower(jive::GuiItem* item)
	{
		auto* activeTreeShower = dynamic_cast<juce::DrawableButton*>(item->getComponent().get());
		jassert(activeTreeShower != nullptr);
		treeShower = activeTreeShower;
	}
	// Default message when no other message is displayed.
	void InfoTabManager::setDefaultState()
	{
		setSingleLineMessage("No Tree Loaded!");
	}

	void InfoTabManager::setSingleLineMessage(juce::String text)
	{
		textItem->state.setProperty("text", text, nullptr);
		textItem->state.setProperty("style", singleLineStyle, nullptr);
	}

	void InfoTabManager::setDoubleLineMessage(juce::String line1, juce::String line2)
	{
		juce::String totalString = line1 + '\n' + line2;
		textItem->state.setProperty("text", totalString, nullptr);
		textItem->state.setProperty("style", doubleLineStyle, nullptr);

	}
}