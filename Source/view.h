#pragma once
#include <jive_core/jive_core.h>

namespace jiveUI
{
	// SUB-COMPONENTS ===========================================================
	
	[[nodiscard]] inline auto midiVelocityLabel()
	{
		return juce::ValueTree{
			"Text",
			{
				{"id", "midiVelocity-label"},

			},
			{
				juce::ValueTree{
					"Text",
					{
						{"text", " velocity"},
					},
				},
			},
		};
	}

	[[nodiscard]] inline auto midiVelocityKnob()
	{
		return juce::ValueTree{
			"Knob",
			{
				{"id", "midiVelocity-knob"},
				{"width", 80},
				{"height", 80}
			},
		};
	}
	[[nodiscard]] inline auto 
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
			},
			{
				midiVelocityKnob(),
				midiVelocityLabel(),
				
				noteRateKnob(),

				noteTypeComboBox(),
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
						{ "background", "#FFFFFF" },
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