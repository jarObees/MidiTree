#pragma once
namespace jive_gui
{
	[[nodiscard]] inline auto getInputBlockBottom()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"width", "100%"},
				{"height", "25%"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::darkGray)},
						},
				},
			}
		};
	}
	[[nodiscard]] inline auto getInputBlockLabel()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"width", "100%"},
				{"height", "50%"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::grey)},
						},
				},
			}
		};
	}

	[[nodiscard]] inline auto getInputBlockTop()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"width", "100%"},
				{"height", "25%"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::white)},
						},
				},
			},
			{

			}
		};
	}

	// Height is defined in getInputRow()
	[[nodiscard]] inline auto getInputBlock()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"width", 25},
				{"height", "100%" },
				{"flex-direction", "column"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::debug_tertiary)},
						},
				},
			},
			{
				getInputBlockTop(),
				getInputBlockLabel(),
				getInputBlockBottom(),
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
				{"justify-content", "space-between"},
				{"width", "100%"},
				{"height", 75},
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

	// MAIN COMPONENT =================================================================
	[[nodiscard]] inline auto getAnalogUserInput()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"flex-direction", "column"},
				{"align-items", "centre"},
				{"justify-content", "space-between"},
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
				getInputRow(),
				getInputRow(),
				getInputRow(),
				getInputRow(),
			}
		};
	}
}