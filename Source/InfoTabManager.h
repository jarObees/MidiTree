#pragma once
#include <JuceHeader.h>

namespace InfoTabManager
{
	class InfoTabManager
	{
	public:
		void connectInfoTab(jive::GuiItem*);
		void connectActiveTreeShower(jive::GuiItem*);
		void setSingleLineMessage(juce::String);
		void setDoubleLineMessage(juce::String line1, juce::String line2);
		void setDefaultState();

	private:
		jive::GuiItem* textItem = nullptr;
		static constexpr auto singleLineStyle = R"(
			{
				"font-family" : "Perpetua Titling MT",
				"foreground" : "white",
				"font-size" : 20,
				"letter-spacing" : 1,
				"font-weight" : "bold",
			}
		)";

		static constexpr auto doubleLineStyle = R"(
			{
				"font-family" : "Perpetua Titling MT",
				"foreground" : "white",
				"font-size" : 11,
				"letter-spacing" : 1,
				"font-weight" : "bold",
			}
		)";
	};
}