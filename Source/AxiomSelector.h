#pragma once

namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace AxiomSelecta
		{
			const int RADIOGROUPNUM = 1;
			// This component is for use within the note wheel, hencethe width and height being at 100%
			class AxiomSelectorView
				: public jive::View
			{
			public:
				AxiomSelectorView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = idRowColMaker(IdPrefix::axiom, rowNum, columnNum);
					bgImage = juce::ImageCache::getFromMemory(BinaryData::NW_OFF_png, BinaryData::NW_ON_pngSize); /// TODO: MAKE NEW IMAGE FOR THIS.
				}

				juce::ValueTree initialise() final
				{
					juce::ValueTree axiomSelectaComponent =
					{
						"Component",
						{
							{"id", id},
							{"display", "block"},
							{"width", "100%"},
							{"height", "100%"},
							{
							"style",
								new jive::Object{
									{"background", jive::toVar(colors::red)},
								}
							},
						},
						{
							juce::ValueTree
							{
								"Image",
								{
									{"source", jive::toVar(bgImage)},
									{"width", "100%"},
									{"height", "100%"},
								}
							},
							juce::ValueTree
							{
								"Button",
								{
									{"centre-x", "50%"},
									{"radio-group", RADIOGROUPNUM},
									{"toggleable", true},
								}
							}
						}
					};
					return axiomSelectaComponent;
				}

				void setup(jive::GuiItem& item) final
				{
					DBG("Setting up the axiom selector...");
					imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source");
					jassert(imageSource != nullptr);

				}

			private:
				juce::String id;
				const int rowNum, columnNum;
				juce::Image bgImage;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
			};

		}
	}
}