#pragma once
#include <JuceHeader.h>

namespace InfoTabManager
{
	class InfoTabManager
	{
	public:
		void connectInfoTab(jive::GuiItem*);
		void setSingleLineMessage(juce::String);
		void setDoubleLineMessage(juce::String, juce::String);
		void setDefaultState();

	private:
		jive::GuiItem* textItem = nullptr;
		static constexpr auto singleLineStyle = R"(
			{
				"font-family" : "Perpetua Titling MT",
				"foreground" : "white",
				"font-size" : 20,
				"letter-spacing" : 1,
			}
		)";

		static constexpr auto doubleLineStyle = R"(
			{
				"font-family" : "Perpetua Titling MT",
				"foreground" : "white",
				"font-size" : 15,
				"letter-spacing" : 1,
			}
		)";
	};
}