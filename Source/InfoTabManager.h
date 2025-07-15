#pragma once
#include <JuceHeader.h>

namespace InfoTabManager
{
	class InfoTabManager
	{
	public:
		void connectInfoTab(jive::GuiItem*);
	private:
		jive::GuiItem* textItem;
		static constexpr auto defaultStyle = R"(
			{
				"foreground" : "red",
				"font-size" : 20,
				"letter-spacing" : 1,
			}
		)";
	};
}