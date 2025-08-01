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
						{"display", "flex"},
						{"flex-direction", "column"},
						{"width", "90%"},
						{"height", "90%"},
						{"centre-x", "50%"},
						{"centre-y", "50%"},
						{"justify-content", "centre"},
						{"align-content", "centre"},
						{"justify-items", "centre"},
						{"align-items", "centre"},
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
						//// Temporary Plant Button
						//jive::makeView<DrawableButton>(32, 32, juce::ImageCache::getFromMemory(BinaryData::button_grow_png,
						//																		BinaryData::button_grow_pngSize), 
						//																		"", 
						//																		StringIds::plantButton),
						//// Temporary bypass Button
						jive::makeView<DrawableToggleButton>(32, 32, 
															 juce::ImageCache::getFromMemory(BinaryData::green_32_32_png, BinaryData::green_32_32_pngSize),
															 juce::ImageCache::getFromMemory(BinaryData::red_32_32_png, BinaryData::red_32_32_pngSize),
															 StringIds::forestBypassButton),
						jive::makeView<DrawableButton>(16, 16, 
													   juce::ImageCache::getFromMemory(BinaryData::ResetButton_png, BinaryData::ResetButton_pngSize), 
													   "",
													   StringIds::forestResetButton),

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
						{"width", "100%"},
						{"flex-direction", "row"},
						{"justify-content", "space-between"},
												{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(jiveGui::colors::debug_secondary)},
								},
						},
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
				return jive::makeView<HorizontalFilmstripKnobView>(252,
												  32,
												  juce::ImageCache::getFromMemory(BinaryData::horiKnob7O_ornate_v2_png,
																				  BinaryData::horiKnob7O_ornate_v2_pngSize),
												  StringIds::forestSlider, 7, true);
			}
		};
	}
}