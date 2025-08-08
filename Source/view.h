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
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::crown_primary)},
						},
				},
			},
			// LEFT CONTAINER
			{
				juce::ValueTree
				{
					"Component",
					{
						{"align-items", "centre"},
						{"flex-direction", "column"},
						{"height", "100%"},
						{"padding", "10px"},
						{"width", "80%"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_highlight)},
								},
						},
					},
					{
						jiveGui::AnalogUserInput::getAnalogUserInput(octavesTest),
						/// TODO: ADD TEXT
						///TODO: ADD RESET BUTTON
					},
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
																"grow!", 
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
		return juce::ValueTree
		{
			"Component",
			{
				{"width", "20%"},
				{"height", "100%"},
				{"flex-direction", "column"},
				{
					"style",
						new jive::Object
						{
							{ "background", jive::toVar(colors::debug_primary)},
						},
				},
			},
			{
				jive::makeView<DrawableButton>(64, 32, filmstripKnob, "save", StringIds::saveButton),
				jive::makeView<ComboBoxView>(64, 64, StringIds::presetComboBox),
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
		juce::Image filmstripKnob = imageCollection.at(ImageIds::mainKnobFilmstrip);
		return juce::ValueTree{
			"Component",
			{
				{"id", "ground"},
				{"flex-direction", "row"},
				{"justify-content", "centre"},
				{"width", "100%"},
				{"height", "13%"},
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::ground_primary)},
						},
				},
			},
			{
				getMidiVelocityKnob(imageCollection),
				getNoteRateKnob(imageCollection),
				jive::makeView<ImageComboBoxView>(32, 32, StringIds::noteTypeComboBox), // TODO: Attach to Params in createEditor() in PluginProcessor.cpp
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