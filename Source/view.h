#pragma once
#include <jive_core/jive_core.h>
#include "Ids.h"
#include "Colors.h"
#include "Images.h"
#include "ComboBoxView.h"
#include "FilmstripKnobView.h"
#include "TextEditorView.h"
#include "DrawableToggleButton.h"
#include "ButtonImageText.h"
#include "ComponentGetter.h"
#include "AnalogUserInput.h"
#include "ComboBoxImageView.h"
#include "Forest.h"
#include "InfoTab.h"

namespace jiveGui
{

	// MAIN COMPONENTS ==========================================================
	[[nodiscard]] inline auto getBuilderComponent(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnob = juce::ImageCache::getFromMemory(BinaryData::filmstripKnob_ornate_png, BinaryData::filmstripKnob_ornate_pngSize);
		juce::Image filmstripKnobHori = imageCollection.at(ImageIds::horiKnobFilmstrip);
		juce::Image octavesTest = imageCollection.at(ImageIds::octavesBackground1);
		return juce::ValueTree{
			"Component",
			{
				{"id", "crown"},
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "row"},
				{"width", "100%"},
				{"height", "60%"},
			},
			// LEFT CONTAINER
			{
				juce::ValueTree
				{
					"Component",
					{
						{"display", "block"},
						{"height", "100%"},
						{"width", "80%"},
					},
					{
						juce::ValueTree
						{
							"Image",
							{
								{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::AUIMainBG_png, BinaryData::AUIMainBG_pngSize))},
								{"width", "100%"},
								{"height", "100%"},
							}
						},
						juce::ValueTree
						{
							"Component",
							{
								{"align-items", "centre"},
								{"flex-direction", "column"},
								{"height", "100%"},
								{"padding", "10px"},
								{"width", "100%"},
							},
							{
								jiveGui::AnalogUserInput::getAnalogUserInput(octavesTest),
								juce::ValueTree
								{
									"Component",
									{
										{"display", "flex"},
										{"width", "100%"},
										{"height", "20"},
										{"justify-content", "flex-start"},
									},
									{
										jive::makeView<DrawableButton>(16, 16,
																		juce::ImageCache::getFromMemory(BinaryData::ResetButton_png, BinaryData::ResetButton_pngSize),
																		StringIds::analogUserInputResetButton),
									}
								}
							},
						}
					}
				},

				// RIGHT CONTAINER
				juce::ValueTree
				{
					"Component",
					{
						{"display", "block"},
						{"width", "20%"},
						{"height", "100%"},
					},
					{
						juce::ValueTree
						{
							"Image",
							{
								{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::AUI_SIDEBAR_png, BinaryData::AUI_SIDEBAR_pngSize))},
								{"width", "100%"},
								{"height", "100%"},
							}
						},
						juce::ValueTree
						{
							"Component",
							{
								{"width", "100%"},
								{"height", "100%"},
								{"align-items", "centre"},
								{"flex-direction", "column"},
							},
							{
								jive::makeView<SquareFilmStripKnobView>(64, 64, 
																  filmstripKnob, 
																  StringIds::generationsKnob),
								jive::makeView<DrawableButton>(64, 64, 
																juce::ImageCache::getFromMemory(BinaryData::button_grow_png, BinaryData::button_grow_pngSize), 
																StringIds::growButton),
							}
						},
					},
				},
			}
		};
	}

	[[nodisacrd]] inline auto getForestLeft(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnobHori = imageCollection.at(ImageIds::horiKnobFilmstrip);
		return juce::ValueTree
		{
			"Component",
			{
				{"display", "block"},
				{"width", "80%"},
				{"height", "100%"},
			},
			{
				juce::ValueTree
				{
					"Image",
					{
						{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::ForestMainBG_png, BinaryData::ForestMainBG_pngSize))},
						{"width", "100%"},
						{"height", "100%"},
					}
				},
				ForestUI::Forest{}(),
			}
		};
	}

	[[nodiscard]] inline auto getForestRight(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnob = imageCollection.at(ImageIds::mainKnobFilmstrip);
		std::vector<juce::Image> blankVector;
		return juce::ValueTree
		{
			"Component",
			{
				{"display", "block"},
				{"width", "20%"},
				{"height", "100%"},
			},
			{
				juce::ValueTree
				{
					"Image",
					{
						{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::ForestRightBG_png, BinaryData::ForestRightBG_pngSize))},
						{"width", "100%"},
						{"height", "100%"},
					}
				},
				juce::ValueTree
				{
					"Component",
					{
						{"width", "100%"},
						{"height", "100%"},
						{"flex-direction", "column"},
						{"justify-content", "space-around"},
						{"align-items", "centre"},
					},
					{
						jive::makeView<DrawableButton>(64,
													   32,
													   juce::ImageCache::getFromMemory(BinaryData::SaveButtonNormal_png, BinaryData::SaveButtonNormal_pngSize),
													   StringIds::saveButton,
													   juce::ImageCache::getFromMemory(BinaryData::SaveButtonHover_png,BinaryData::SaveButtonHover_pngSize),
													   juce::ImageCache::getFromMemory(BinaryData::SaveButtnoPressed_png, BinaryData::SaveButtnoPressed_pngSize)),
						jive::makeView<ImageComboBoxView>(64, 64, StringIds::presetComboBox,
														  juce::ImageCache::getFromMemory(BinaryData::SeedbankButtonNormal_png, BinaryData::SeedbankButtonNormal_pngSize),
														  blankVector,
														  juce::ImageCache::getFromMemory(BinaryData::SeedbankButtonHover_png, BinaryData::SeedbankButtonHover_pngSize),
														  juce::ImageCache::getFromMemory(BinaryData::SeedbankButtonPressed_png, BinaryData::SaveButtnoPressed_pngSize)),
					}
				}
			}
		};
	}

	[[nodiscard]] inline auto getForestComponent(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"id", "trunk"},
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "row"},
				{"width", "100%"},
				{"height", "17%"},
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::sky_primary)},
						},
				},
			},
			{
				getForestLeft(imageCollection),
				getForestRight(imageCollection),
			}
		};
	}
	[[nodiscard]] inline auto getGround(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		std::vector<juce::Image> orderedNotetypeImageCollection;
		orderedNotetypeImageCollection.push_back(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorBase_png, BinaryData::NoteSelectorBase_pngSize));
		orderedNotetypeImageCollection.push_back(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorQuarter_png, BinaryData::NoteSelectorQuarter_pngSize));
		orderedNotetypeImageCollection.push_back(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorDotted_png, BinaryData::NoteSelectorDotted_pngSize));
		orderedNotetypeImageCollection.push_back(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorTriplet_png, BinaryData::NoteSelectorTriplet_pngSize));

		return juce::ValueTree
		{
			"Component",
			{
				{"display", "block"},
				{"width", "100%"},
				{"height", "13%"},
			},
			{
				juce::ValueTree
				{
					"Image",
					{
						{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::GroundBottomBG_png, BinaryData::GroundBottomBG_pngSize))},
						{"width", "100%"},
						{"height", "100%"},
					}
				},
				juce::ValueTree
				{
					"Component",
					{
						{"id", "ground"},
						{"flex-direction", "row"},
						{"justify-content", "space-around"},
						{"align-items", "centre"},
						{"centre-x", "50%"},
						{"centre-y", "50%"},
						{"width", "100%"},
						{"height", "100%"},
					},
					{
						jive::makeView<ImageComboBoxView>(32, 
														  32, 
														  StringIds::noteTypeComboBox, 
														  juce::ImageCache::getFromMemory(BinaryData::NoteSelectorBase_png, 
																						  BinaryData::NoteSelectorBase_pngSize),
														  orderedNotetypeImageCollection), // TODO: Attach to Params in createEditor() in PluginProcessor.cpp
						getMidiVelocityKnob(imageCollection),
						getNoteRateKnob(imageCollection),
					}
				},
			}
		};
	}

	[[nodiscard]] inline auto getInfoComponent(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		return jiveGui::InfoTabUI::InfoTab{}();
	}

	// TOP LEVEL VIEW =========================================================
	[[nodiscard]] inline auto getView(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		//TODO: PASS IN THE IMAGES TO THE COMPONENTS.
		return juce::ValueTree{
			"Editor",
			{
				{"width", 350},
				{"height", 700},
				{"align-items", "flex-start"},
				{"justify-content", "centre"},
				{"flex-direction", "column"},
				{
					"style",
					new jive::Object{
						{"font-family", "Perpetua Titling MT"},
						{ "background", "#000000" },
						{ "foreground", "#FFFFFF"},
					},
				},
			},
			{
				getBuilderComponent(imageCollection),
				getForestComponent(imageCollection),
				getInfoComponent(imageCollection),
				getGround(imageCollection),
			}
		};
	}
}