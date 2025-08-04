#include "InfoTabManager.h"
namespace InfoTabManager
{
	InfoTabManager::InfoTabManager(Tree::ActiveTreeManager* _activeTreeManager)
	{
		_activeTreeManager->addListener(this);
	}

	void InfoTabManager::onActiveTreeChanged(std::shared_ptr <Tree::MidiTree>)
	{
		DBG("Active Tree Changed!");

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