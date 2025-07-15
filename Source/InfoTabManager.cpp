#include "InfoTabManager.h"
namespace InfoTabManager
{
	void InfoTabManager::connectInfoTab(jive::GuiItem* item)
	{
		textItem = item;
		setDefaultState();
	}
	// Default message when no other message is displayed.
	void InfoTabManager::setDefaultState()
	{
		/// TODO: ADD CHECK IF TREE IS LOADED FIRST
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