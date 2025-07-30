#pragma once
#include <juce_data_structures/juce_data_structures.h>
#include "FilmstripKnobView.h"

// Contains the actual getters for components and stuff to keep it clean.

namespace jiveGui
{
	std::unordered_map<std::string, juce::Image> imageMap;
	[[noexcept]] inline auto getMidiVelocityKnob(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "column"},
			},
			{
				jive::makeView<SquareFilmStripKnobView>(64, 64, imageCollection[ImageIds::mainKnobFilmstrip], StringIds::midiVelocityKnob),
				juce::ValueTree
				{
					"Text",
					{
						{"id", StringIds::midiVelocityKnobText},
						{"flex-direction", "column"},
						{
						"style",
							new jive::Object
								{
									{ "foreground", "#FFFFFF" },
									{ "font-size", 20 },
									{ "letter-spacing", 1.5 },
								},
						},
					},	
				},
				{
					juce::ValueTree
					{
						"Text",
						{
							{"text", "velocity"},
							{
								"style",
									new jive::Object
										{
											{ "foreground", "#FFFFFF" },
											{ "font-size", 10 },
											{ "letter-spacing", 1.5 },
										},
							},
						}
					},
				}
			}
		};
	}

	[[noexcept]] inline auto getNoteRateKnob(std::unordered_map<std::string, juce::Image>& imageCollection)
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "column"},
			},
			{
				jive::makeView<SquareFilmStripKnobView>(64, 64, imageCollection[ImageIds::mainKnobFilmstrip], StringIds::noteRateKnob),
				juce::ValueTree
				{
					"Text",
					{
						{"id", StringIds::noteRateKnobText},
						{"flex-direction", "column"},
						{
						"style",
							new jive::Object
								{
									{ "foreground", "#FFFFFF" },
									{ "font-size", 20 },
									{ "letter-spacing", 1.5 },
								},
						},
					},
				},
				{
					juce::ValueTree
					{
						"Text",
						{
							{"text", "Note Rate"},
							{
								"style",
									new jive::Object
										{
											{ "foreground", "#FFFFFF" },
											{ "font-size", 10 },
											{ "letter-spacing", 1.5 },
										},
							},
						}
					},
				}
			}
		};
	}
}