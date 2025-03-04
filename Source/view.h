#pragma once
#include <jive_core/jive_core.h>
#include "knob_view.h"  

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
			},
			{
				// Get Crown components.
			}
		};
	}
	[[nodiscard]] inline auto getTrunk()
	{
		return juce::ValueTree{
			"Component",
			{
				{"id", "trunk"},
				{"align-items", "centre"},
				{"justify-content", "centre"},
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
				{"align-content", "space-around"},
				{"justify-content", "centre"},
				{
					"style",
						new jive::Object{
							{ "background", "#000000" },
						},
				},
			},
			{
				// Midi Velocity Knob
				KnobWithLabel{}
				.withId("midiVelocity-knob")
				.withLabelId("midiVelocity-label")
				.withLabel(" velocity")()
			}
		};
	}

	// Top Level View =========================================================
	[[nodiscard]] inline auto getView()
	{
		return juce::ValueTree{
			"Editor",
			{
				{"width", 300},
				{"height", 600},
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{
					"style",
					new jive::Object{
						{ "background", "#000000" },
						{ "foreground", "#FFFFFF"},
					},
				},
			},
			{
				getGround(),
				getTrunk(),
				getCrown(),
			}
		};
	}
}