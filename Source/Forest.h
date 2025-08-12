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
						{"height", "72%"},
						{"centre-x", "50%"},
						{"centre-y", "50%"},
						{"justify-content", "space-between"},
						{"align-content", "centre"},
						{"justify-items", "centre"},
						{"align-items", "centre"},
					},
					{
						getTreeRow(),
						getForestSlider(),
						getBottomRow(),
					}
				};
			}
		private:
			juce::ValueTree getBottomRow() const
			{
				juce::ValueTree bottomTray
				{
					"Component",
					{
						{"width", "100%"},
						{"flex-direction", "row"},
						{"justify-content", "space-between"},
						{"align-items", "centre"},
					},
				};
				bottomTray.addChild(jive::makeView<DrawableButton>(106, 
																   32, juce::Image{},
																   StringIds::plantButton), -1, nullptr);
				bottomTray.addChild(jive::makeView<DrawableToggleButton>(32, 
																		 16,
																		 juce::ImageCache::getFromMemory(BinaryData::ForestBypassOn_png, BinaryData::ForestBypassOn_pngSize),
																		 juce::ImageCache::getFromMemory(BinaryData::ForestBypassOff_png, BinaryData::ForestBypassOff_pngSize),
																		 StringIds::forestBypassButton), -1, nullptr);
				bottomTray.addChild(jive::makeView<DrawableButton>(106, 
																   32,
																   juce::Image{},
																   StringIds::forestResetButton), -1, nullptr);
				return bottomTray;
			}
			juce::ValueTree getTreeRow() const // Returns row of the little tree guys.
			{
				juce::ValueTree treeRow
				{
					"Component",
					{
						{"width", "100%"},
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
				return jive::makeView<HorizontalFilmstripKnobView>(252,
												  32,
												  juce::ImageCache::getFromMemory(BinaryData::horiKnob7O_ornate_v2_png,
																				  BinaryData::horiKnob7O_ornate_v2_pngSize),
												  StringIds::forestSlider, 7, true);
			}
		};
	}
}