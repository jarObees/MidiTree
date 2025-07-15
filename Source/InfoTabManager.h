#pragma once
#include <JuceHeader.h>

namespace InfoTabManager
{
	class InfoTabManager
	{
	public:
		void connectInfoTab(jive::GuiItem*);
	private:
		jive::GuiItem* textItem = nullptr;
		static constexpr auto defaultStyle = R"(
			{
				"font-family" : "Perpetua Titling MT",
				"foreground" : "white",
				"font-size" : 20,
				"letter-spacing" : 1,
			}
		)";
	};
}