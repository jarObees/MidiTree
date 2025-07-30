#pragma once
#include "Tree.h"

namespace jiveGui
{
	namespace ForestUI
	{
		class Forest
		{
		public:
			[[nodiscard]] auto operator()() const
			{
				return juce::ValueTree
				{
					"Component",
					{
						{"width", "90%"},
						{"height", "90%"},
						{"centre-x", "50%"},
						{"centre-y", "50%"},
						{"justify-content", "space-between"},
						{"align-content", "space-between"},
						{"justify-items", "centre"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(jiveGui::colors::ground_tertiary)},
								},
						},
					},
					{
						getTreeRow(),
						getForestSlider(),
						// Temporary Plant Button
						jive::makeView<DrawableButton>(32, 32, juce::ImageCache::getFromMemory(BinaryData::button_grow_png,
																								BinaryData::button_grow_pngSize), 
																								"", 
																								StringIds::plantButton),
						// Temporary bypass Button
						jive::makeView<DrawableButton>(32, 32, juce::ImageCache::getFromMemory(BinaryData::button_grow_png,
																								BinaryData::button_grow_pngSize),
																								"",
																								StringIds::forestBypassButton),

					}
				};
			}
		private:
			juce::ValueTree getTreeRow() const // Returns row of the little tree guys.
			{
				juce::ValueTree treeRow
				{
					"Component",
					{
						{"flex-direction", "row"},
						{"justify-content", "space-between"},
					},
				};

				for (int i = 0; i < 7; ++i)
				{
					treeRow.addChild(jive::makeView<TreeComponentView>(i), -1, nullptr);
				}
				return treeRow;
			}

			juce::ValueTree getForestSlider() const
			{
				return jive::makeView<HorizontalFilmstripKnobView>(152,
												  32,
												  juce::ImageCache::getFromMemory(BinaryData::horiKnob7O_ornate_png,
																				  BinaryData::horiKnob7O_ornate_pngSize),
												  StringIds::forestSlider, 7, true);
			}
		};
	}
}