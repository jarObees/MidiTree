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
#include "ComponentGetter.h"
#include "AnalogUserInput.h"

namespace jiveGui
{

	// MAIN COMPONENTS ==========================================================
	[[nodiscard]] inline auto getCrown(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		juce::Image filmstripKnob = imageCollection.at(ImageIds::mainKnobFilmstrip);
		juce::Image filmstripKnobHori = imageCollection.at(ImageIds::horiKnobFilmstrip);
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
						jive::makeView<FilmstripKnobView>(224, 32, filmstripKnobHori, StringIds::forestSlider, true),
						jiveGui::AnalogUserInput::getAnalogUserInput(),
					},
				},

				// RIGHT CONTAINER
				juce::ValueTree
				{
					"Component",
					{
						{"align-items", "centre"},
						{"height", "100%"},
						{"padding", "10px"},
						{"width", "20%"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_secondary)},
								},
						},
					},
					{		
						jive::makeView<ComboBoxView>(40, 10, StringIds::presetComboBox),
						jive::makeView<SaveButton>(20, 40, filmstripKnob, "save", StringIds::saveButton),
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
		juce::Image filmstripKnob = imageCollection.at(ImageIds::mainKnobFilmstrip);
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
				getMidiVelocityKnob(imageCollection),
				getNoteRateKnob(imageCollection),
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
				getCrown(imageCollection),
				getSky(imageCollection),
				getGround(imageCollection),
			}
		};
	}
}