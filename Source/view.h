#pragma once
#include <jive_core/jive_core.h>
#include "Ids.h"
#include "Colors.h"
#include "Images.h"
#include "ComboBoxView.h"
#include "FilmstripKnobView.h"
#include "TextEditorView.h"
#include "Button.h"
#include "SaveButtonView.h"

namespace jive_gui
{
	// MAIN COMPONENTS ==========================================================
	[[nodiscard]] inline auto getCrown(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnob = imageCollection.at("knobTestWhite");
		juce::Image filmstripKnobHori = imageCollection.at("horiSliderWhite");
		return juce::ValueTree{
			"Component",
			{
				{"id", "crown"},
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "row"},
				{"width", "100%"},
				{"height", "70%"},
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
						{"flex-grow", 1.5}, //TODO: Adjust flex grow so that it's more fixed.
						{"flex-direction", "column"},
						{"height", "100%"},
						{"padding", "10px"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_highlight)},
								},
						},
					},
					{
						jive::makeView<FilmstripKnobView>(224, 32, filmstripKnobHori, StringIds::forestSlider, true),
						jive::makeView<TextEditorView>(80, 160, StringIds::rulesetTextbox, "100%", "50%"),
					},
				},

				// RIGHT CONTAINER
				juce::ValueTree
				{
					"Component",
					{
						{"align-items", "centre"},
						{"flex-grow", 1},
						{"height", "100%"},
						{"padding", "10px"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_secondary)},
								},
						},
					},
					{
						// Button{}.withHeight(20).withWidth(40).withId("save-button").withText("save")(),
		
						jive::makeView<ComboBoxView>(40, 10, StringIds::presetComboBox),
						jive::makeView<SaveButton>(20, 40, filmstripKnob, "save", StringIds::saveButton),
						jive::makeView<TextEditorView>(0, 0, StringIds::axiomTextBox, "100%", "10%"),
						jive::makeView<SaveButton>(20, 40, filmstripKnob, "plant!", StringIds::plantButton),
						jive::makeView<FilmstripKnobView>(64, 64, filmstripKnob, StringIds::generationsKnob),
						jive::makeView<SaveButton>(20, 40, filmstripKnob, "grow!", StringIds::growButton),
					},
				},
			}
		};
	}
	
	[[nodiscard]] inline auto getSky(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		return juce::ValueTree{
			"Component",
			{
				{"id", "trunk"},
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"width", "100%"},
				{"height", "10%"},
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::sky_primary)},
						},
				},
			},
			{
				//Get Trunk Components
			}
		};
	}
	
	[[nodiscard]] inline auto getGround(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnob = imageCollection.at("knobTestWhite");
		return juce::ValueTree{
			"Component",
			{
				{"id", "ground"},
				{"flex-direction", "row"},
				{"justify-content", "centre"},
				{"width", "100%"},
				{"height", "20%"},
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::ground_primary)},
						},
				},
			},
			{
				jive::makeView<FilmstripKnobView>(64, 64, filmstripKnob, StringIds::midiVelocityKnob),
				jive::makeView<FilmstripKnobView>(64, 64, filmstripKnob, StringIds::noteRateKnob),
				jive::makeView<ComboBoxView>(80, 20, StringIds::noteTypeComboBox), // TODO: Attach to Params in createEditor() in PluginProcessor.cpp
			}
		};
	}

	// TOP LEVEL VIEW =========================================================
	[[nodiscard]] inline auto getView(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		//TODO: PASS IN THE IMAGES TO THE COMPONENTS.
		return juce::ValueTree{
			"Editor",
			{
				{"width", 300},
				{"height", 600},
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
				getCrown(imageCollection),
				getSky(imageCollection),
				getGround(imageCollection),
			}
		};
	}
}