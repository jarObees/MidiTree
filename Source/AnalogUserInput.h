#pragma once
#include "NoteWheel.h"
#include "InputBlockTop.h"
#include "DirectionInput.h"

// NOTE: For each block, the id will be "ipxy", where x is the row number, and y is the column (0 indexed). 
// If y = -1, then it's the LH side of a ruleset.
namespace jiveGui
{
	namespace AnalogUserInput
	{
		const int NUMBLOCKCOLUMNS = 8;
		const int NUMBLOCKROWS = 5;
		const int BLOCKHEIGHT = 75;
		const int BLOCKWIDTH = 25;

		[[nodiscard]] inline auto getInputBlockBottom(int rowNum, int columnNum)
		{
			const juce::String id = rowColIdMaker(jiveGui::IdPrefix::inputBlockBottom, rowNum, columnNum);
			juce::ValueTree inputBlockBottomComponent =
			{
				"Component",
				{
					{"id", id},
					{"width", "100%"},
					{"height", "25%"},
					{"orientation", "vertical"},
					{"align-content", "centre"},
					{"justify-content", "centre"},
				}

			};
			inputBlockBottomComponent.addChild(jive::makeView<DirectionInput::DirectionInputView>(rowNum, columnNum),
											   -1,
											   nullptr);
			return inputBlockBottomComponent;
		}

		[[nodiscard]] inline auto getNoteWheel(int rowNum, int columnNum)
		{
			return jive::makeView<NoteWheel::NoteWheelView>(rowNum, columnNum);
		}

		//TODO: Fix how the axiom is displayed along with octaves here.
		[[nodiscard]] inline auto getInputBlockTop(int rowNum, int columnNum)
		{
			const juce::String id = rowColIdMaker(IdPrefix::inputBlockTop, rowNum, columnNum);
			juce::ValueTree rootComponent
			{
				"Component",
				{
					{"id", id},
					{"width", "100%"},
					{"height", "25%"},
					{"orientation", "vertical"},
					{"align-items", "centre"},
					{"justify-content", "centre"},
				},
			};

			if (columnNum == -1)
			{
				DBG("BUILDING WITH AXIOM");
				auto axiomView = jive::makeView<InputBlockTop::AxiomSelectorView>(rowNum, columnNum); // Add this as a child 
				rootComponent.addChild(axiomView, -1, nullptr);
			}

				DBG("BUILDING OCTAVES THING");
				rootComponent.addChild(jive::makeView<InputBlockTop::OctavesInputView>(rowNum, columnNum),
									   -1,
									   nullptr); // Always add this as a child regardless.
				return rootComponent;
		}

		// Height is defined in getInputRow()
		// If rowNum == -1, then it indicates that inputBlock is a LH rule.
		// Children order: BlockTop, noteWheel, BlockBottom
		[[nodiscard]] inline auto getInputBlock(int rowNum, int columnNum)
		{
			const juce::String id = rowColIdMaker(IdPrefix::inputBlock, rowNum, columnNum);
			return juce::ValueTree
			{
				"Component",
				{
					{"id", id},
					{"width", BLOCKWIDTH},
					{"height", "100%" },
					{"flex-direction", "column"},
					{"style",
							new jive::Object
							{
								{ "background", jive::toVar(colors::debug_tertiary)},
							},
					},
				},
				{
					getInputBlockTop(rowNum, columnNum),
					getNoteWheel(rowNum, columnNum),
					getInputBlockBottom(rowNum, columnNum),
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
		// Children  (in order) = inputBlock, inputBlockSpacer, n inputBlocks
		[[nodiscard]] inline auto getInputRow(int rowNum)
		{
			const juce::String idTing = rowColIdMaker(IdPrefix::inputRow, rowNum, 0); // Id: "irx0"
			juce::ValueTree row
			{
				"Component",
				{
					{"id", idTing},
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

			row.addChild(getInputBlock(rowNum, -1), -1, nullptr);
			row.addChild(getInputBlockSpacer(), -1, nullptr);
			for (int j = 0; j < NUMBLOCKCOLUMNS; ++j)
			{
				row.addChild(getInputBlock(rowNum, j), -1, nullptr);
			}

			return row;
		}

		// MAIN COMPONENT =================================================================
		// Children: n InputRows;
		[[nodiscard]] inline juce::ValueTree getAnalogUserInput()
		{
			DBG("Getting analog user input");
			juce::ValueTree slots
			{
				"Component",
				{
					{"id", StringIds::analogUserInput},
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