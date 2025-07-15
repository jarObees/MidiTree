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
						{"height", "8%"},
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
								{"height", "100%"},
								{"text", jive::toVar(text)},
								{"justification", juce::Justification::centred},
								{"align-content", "centre"},
								{"justify-content", "centre"},
								{
								"style",
									new jive::Object
										{
											//{"background", jive::toVar(jiveGui::colors::debug_secondary)},
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
						///TODO: ADD DYNAMIC ACTIVE IMAGE. bool state image that indicates whether the thing is active or not.
					}
				};
			}
		};
	}
}