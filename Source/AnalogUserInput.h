#pragma once
namespace jive_gui
{
	[[nodiscard]] inline auto getInputBlock()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"width", 20},
				{"height", "100%" },
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::debug_tertiary)},
						},
				},
			}
		};

	}

	[[nodiscard]] inline auto getInputRow()
	{
		const int NUMBLOCKS = 10;
		juce::ValueTree row
		{
			"Component",
			{
				{"flex-direction", "row"},
				{"justify-content", "flex-start"},
				{"width", "100%"},
				{"height", 60},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::debug_secondary)},
						},
				},
			},
		};

		// Add n instances of getInputBlock() as children
		for (int i = 0; i < NUMBLOCKS; ++i)
		{
			row.addChild(getInputBlock(), -1, nullptr);
		}

		return row;
	}

	// MAIN COMPONENT
	[[nodiscard]] inline auto getAnalogUserInput()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"flex-direction", "column"},
				{"align-items", "centre"},
				{"justify-content", "flex-start"},
				{"width", "100%"},
				{"height", "80%"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::debug_primary)},
						},
				},
			},
			{
				getInputRow(),
			}
		};
	}
}