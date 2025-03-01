#pragma once
#include <jive_core/jive_core.h>

namespace jiveUI
{
	// Used in plugin editor for the view.
	[[nodiscard]] inline auto getView()
	{
		return juce::ValueTree{
			"Editor",
			{
				{"width", 800},
				{"height", 250},
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
				// TODO: Create other objects here throuhg functions that can be defined earlier.
			}
		};
	}
}