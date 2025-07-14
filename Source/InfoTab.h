#pragma once

namespace jiveGui
{
	namespace InfoTabUI
	{
		class InfoTab
		{
		public:
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
		private:
			[[nodiscard]] juce::ValueTree getInfoLeft() const
			{
				return juce::ValueTree
				{
					"Component",
					{
						{"width", "80%"},
						{"height", "100%"},
						{
							"style",
								new jive::Object
								{
									{ "background", jive::toVar(colors::crown_highlight)},
								},
						},
					},
					{
						///TODO: ADD DYNAMIC NAME TEXT.
						///TODO: ADD DYNAMIC GEN NOTES TEXT.
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