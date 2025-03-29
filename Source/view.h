#pragma once
#include <jive_core/jive_core.h>
#include "Colors.h"
#include "Images.h"
#include "ComboBoxView.h"
#include "FilmstripKnobView.h"
#include "TextEditor.h"
#include "Button.h"
#include "SaveButton.h"

namespace jive_gui
{
	namespace stringIds
	{
		const juce::String forestSlider{ "forest-slider" };
		const juce::String saveButton{ "save-button" };
		const juce::String rulesetTextbox{ "ruleset-textBox" };
		const juce::String loadButton{ "load-button" };
		const juce::String generationsKnob{ "generations-knob" };
		const juce::String growButton{ "grow-button" };
		const juce::String midiVelocityKnob{ "midiVelocity-knob" };
		const juce::String noteRateKnob{ "noteRate-knob" };
		const juce::String noteTypeComboBox{ "noteType-comboBox" };
		const juce::String nameTextBox{ "name-textBox" };
		const juce::String axiomTextBox{ "axiom-textBox" };

	}
	// SUB-COMPONENTS ===========================================================
	[[nodiscard]] inline auto noteRateLabel()
	{
		return juce::ValueTree{
			"Text",
			{
				{"id", "noteRate-label"},

			},
			{
				juce::ValueTree{
					"Text",
					{
						{"text", " rate"},
					},
				},
			},
		};
	}

	[[nodiscard]] inline auto noteRateKnob()
	{
		return juce::ValueTree{
			"Knob",
			{
				{"id", "noteRate-knob"},
				{"width", 80},
				{"height", 80}
			},
		};
	}
	
	[[nodiscard]] inline auto noteTypeComboBox()
	{
		return juce::ValueTree{
			"ComboBox",
			{
				{"id", "noteType-comboBox"},
				{"width", 80},
				{"height", 80}
			},
		};
	}

	// MAIN COMPONENTS ==========================================================
	[[nodiscard]] inline auto getCrown()
	{
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
						jive::makeView<FilmstripKnobView>(224, 32, jive_gui::images::horiSliderWhiteFilmstrip, stringIds::forestSlider, true),
						jive::makeView<TextEditorView>(80, 160, stringIds::rulesetTextbox, "100%", "50%"),
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
						jive::makeView<TextEditorView>(0, 0, stringIds::nameTextBox, "100%", "10%"),
						jive::makeView<SaveButton>(20, 40, jive_gui::images::knobTestWhiteFilmstrip, "save", stringIds::saveButton),
						jive::makeView<TextEditorView>(0, 0, "axiom-textBox", "100%", "10%"),
						jive::makeView<SaveButton>(20, 40, jive_gui::images::knobTestWhiteFilmstrip, "plant!", stringIds::loadButton),
						jive::makeView<FilmstripKnobView>(64, 64, jive_gui::images::knobTestWhiteFilmstrip, stringIds::generationsKnob),
						jive::makeView<SaveButton>(20, 40, jive_gui::images::knobTestWhiteFilmstrip, "grow!", stringIds::growButton),
					},
				},
			}
		};
	}
	
	[[nodiscard]] inline auto getSky()
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
	
	[[nodiscard]] inline auto getGround()
	{
		juce::StringArray comboBoxTypes = { "Quarter", "Dotted" };
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
				jive::makeView<FilmstripKnobView>(64, 64, jive_gui::images::knobTestWhiteFilmstrip, stringIds::midiVelocityKnob),
				jive::makeView<FilmstripKnobView>(64, 64, jive_gui::images::knobTestWhiteFilmstrip, stringIds::noteRateKnob),
				jive::makeView<ComboBoxView>(80, 20, comboBoxTypes, stringIds::noteTypeComboBox), // TODO: Attach to Params in createEditor() in PluginProcessor.cpp
			}
		};
	}

	// TOP LEVEL VIEW =========================================================
	[[nodiscard]] inline auto getView()
	{
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
				getCrown(),
				getSky(),
				getGround(),
			}
		};
	}
}