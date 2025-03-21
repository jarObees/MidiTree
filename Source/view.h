#pragma once
#include <jive_core/jive_core.h>
#include "Colors.h"
#include "Images.h"
#include "FilmstripKnobView.h"

namespace jiveUI
{
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
				{"width", "100%"},
				{"height", "70%"},
				{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::crown_primary)},
						},
				},

			},
			{
				// Get Crown components.
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
				// Midi Velocity Knob
				// jive::makeView<FilmstripKnobView>(images::knobTestWhiteFilmstrip, "midiVelocity-knob"),
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