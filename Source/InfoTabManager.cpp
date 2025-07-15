#include "InfoTabManager.h"
namespace InfoTabManager
{
	void InfoTabManager::connectInfoTab(jive::GuiItem* item)
	{
		textItem = item;
		
		// TEST
		textItem->state.setProperty("style", defaultStyle, nullptr);
		textItem->state.setProperty("text", "BUTT SEX", nullptr);
	}
}