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
				{
					juce::ValueTree
					{
						"Text",
						{
							{"text", "Velocity"},
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
				},
				jive::makeView<SquareFilmStripKnobView>(64, 
														64, 
														juce::ImageCache::getFromMemory(BinaryData::filmstripKnob_ornate_png, BinaryData::filmstripKnob_ornate_pngSize), 
														StringIds::midiVelocityKnob),
			}
		};
	}

	[[noexcept]] inline auto getGenerationsKnob()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "column"},
				{"padding", "10"},

			},
			{
				{
					juce::ValueTree
					{
						"Text",
						{
							{"text", "Gens"},
							{
								"style",
									new jive::Object
										{
											{ "background", "rgba(255, 255, 255, 0.7)"},
											{ "foreground", "#000000" },
											{ "font-size", 9 },
											{ "letter-spacing", 0 },
										},
							},
						}
					},
				},
				jive::makeView<SquareFilmStripKnobView>(64,
														64,
														juce::ImageCache::getFromMemory(BinaryData::filmstripKnob_ornate_png, BinaryData::filmstripKnob_ornate_pngSize),
														StringIds::generationsKnob),
			}
		};
	}

	[[noexcept]] inline auto getGrowButton()
	{
		return juce::ValueTree
		{
			"Component",
			{
				{"align-items", "centre"},
				{"justify-content", "centre"},
				{"flex-direction", "column"},
				{"padding", "10 10"},
			},
			{
				{
					juce::ValueTree
					{
						"Text",
						{
							{"text", "Grow"},
							{
								"style",
									new jive::Object
										{
											{ "background", "rgba(255, 255, 255, 0.7)"},
											{ "foreground", "#000000" },
											{ "font-size", 9 },
											{ "letter-spacing", 0 },
										},
							},
						}
					},
				},
				jive::makeView<DrawableButton>(64, 64,
												juce::ImageCache::getFromMemory(BinaryData::button_grow_png, BinaryData::button_grow_pngSize),
												StringIds::growButton),
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
				jive::makeView<SquareFilmStripKnobView>(64, 
														64, 
														juce::ImageCache::getFromMemory(BinaryData::filmstripKnob_ornate_png, BinaryData::filmstripKnob_ornate_pngSize), 
														StringIds::noteRateKnob),
			}
		};
	}
}