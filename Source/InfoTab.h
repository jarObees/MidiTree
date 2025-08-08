#pragma once
#include "Ids.h"

namespace jiveGui
{
	namespace InfoTabUI
	{

		class InfoTab
		{
		public:
			InfoTab()
			{
				text = "TEST TEXT";
			}

			[[nodiscard]] auto operator()() const
			{
				return juce::ValueTree {
					"Component",
					{
						{"id", "info"},
						{"align-items", "centre"},
						{"justify-content", "centre"},
						{"flex-direction", "row"},
						{"width", "100%"},
						{"height", "10%"},
						{
							"style",
								new jive::Object{
									{ "background", jive::toVar(colors::debug_secondary)},
								},
						},
					},
					{
						getInfoLeft(),
						getInfoRight(),
					}
				};
			}
			juce::String text;
		private:
			[[nodiscard]] juce::ValueTree getInfoLeft() const
			{
				return juce::ValueTree
				{
					"Component",
					{
						{"width", "80%"},
						{"height", "100%"},
						{"display", "block"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_highlight)},
								},
						},
					},
					{
						juce::ValueTree
						{
							"Image",
							{
								{"width", "100%"},
								{"height", "100%"},
								{"source", jive::toVar(juce::ImageCache::getFromMemory(BinaryData::InfoTabLeft_png,
																					   BinaryData::InfoTabLeft_pngSize))
								}
							}
						},
						juce::ValueTree
						{
							"Text",
							{
								{"id", InfoTabUI::textComponentID},
								{"width", "100%"},
								{"x", "5%"},
								{"y", "6%"},
								{"height", "100%"},
								{"text", jive::toVar(text)},
								{"justification", "centred-left"},
								{
								"style",
									new jive::Object
										{
											{ "foreground", "#FFFFFF"},
											{ "font-size", 20 },
											{ "letter-spacing", 1 },
										},
								},
							},
						},

					}
				};
			}
			[[nodiscard]] juce::ValueTree getInfoRight() const
			{
				return juce::ValueTree
				{
					"Component",
					{
						{"width", "20%"},
						{"height", "100%"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_secondary)},
								},
						},
					},
					{
						jive::makeView<DrawableToggleButton>(64, 64, 
															 juce::ImageCache::getFromMemory(BinaryData::MainTreeOn_png, BinaryData::MainTreeOn_pngSize),
															 juce::ImageCache::getFromMemory(BinaryData::MainTreeOff_png, BinaryData::MainTreeOff_pngSize),
															 StringIds::activeTreeShower,
															 false),
					}
				};
			}
		};
	}
}