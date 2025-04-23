#pragma once
#include "NoteWheel.h"

// NOTE: For each block, the id will be "ipxy", where x is the row number, and y is the column (0 indexed). 
// If y = -1, then it's the LH side of a ruleset.
namespace jive_gui
{
	namespace AnalogUserInput
	{

		const int NUMBLOCKCOLUMNS = 8;
		const int NUMBLOCKROWS = 5;
		const int BLOCKHEIGHT = 75;
		const int BLOCKWIDTH = 25;

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

		[[nodiscard]] inline auto getNoteWheel()
		{
			return jive::makeView<NoteWheel::NoteWheelView>();
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
		// If rowNum == -1, then it indicates that inputBlock is an LH thing.
		[[nodiscard]] inline auto getInputBlock(int rowNum, int columnNum)
		{
			juce::String id = "ip" + std::to_string(rowNum) + std::to_string(columnNum);
			return juce::ValueTree
			{
				"Component",
				{
					{"id", id},
					{"width", BLOCKWIDTH},
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
					getNoteWheel(),
					getInputBlockBottom(),
				}
			};
		}

		// Height is defined in getInputRow()
		[[nodiscard]] inline auto getInputBlockSpacer()
		{
			return juce::ValueTree
			{
				"Component",
				{
					{"width", BLOCKWIDTH},
					{"height", "100%" },
					{"flex-direction", "column"},
					{
						"style",
							new jive::Object
							{
								{ "background", jive::toVar(colors::white)},
							},
					},
				},
			};
		}

		// Composes each row. 
		[[nodiscard]] inline auto getInputRow(int i)
		{
			juce::ValueTree row
			{
				"Component",
				{
					{"flex-direction", "row"},
					{"justify-content", "space-between"},
					{"width", "100%"},
					{"height", BLOCKHEIGHT},
					{
						"style",
							new jive::Object
							{
								{ "background", jive::toVar(colors::debug_secondary)},
							},
					},
				},
			};

			row.addChild(getInputBlock(i, -1), -1, nullptr);
			row.addChild(getInputBlockSpacer(), -1, nullptr);
			for (int j = 0; j < NUMBLOCKCOLUMNS; ++j)
			{
				row.addChild(getInputBlock(i, j), -1, nullptr);
			}

			return row;
		}

		// MAIN COMPONENT =================================================================
		[[nodiscard]] inline auto getAnalogUserInput()
		{
			juce::ValueTree slots
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
			};
			
			// Adds each row of blocks to the bigger AnalogUserInput component.
			for (int i = 0; i < NUMBLOCKROWS; ++i)
			{
				slots.addChild(getInputRow(i), -1, nullptr);
			}
			return slots;
		}
	}
}