#pragma once

namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace AxiomSelecta
		{
			const int RADIOGROUPNUM = 1;
			// This component is for use within the note wheel, hence the width and height being at 100%
			class AxiomSelectorView
				: public jive::View
			{
			public:
				AxiomSelectorView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					DBG("Making an AxiomSelecta!");
					id = idRowColMaker(IdPrefix::axiomToggle, rowNum, columnNum);
					bgImage = juce::Image();
				}

				juce::ValueTree initialise() final
				{
					juce::ValueTree axiomSelectaComponent =
					{
						"Component",
						{
							{"display", "block"},
							{"width", "100%"},
							{"height", "100%"},
									//							{
									//"style",
									//	new jive::Object{
									//		{"background", jive::toVar(colors::debug_secondary)},
									//	}
									//},
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
									{"id", id},
									{"width", "100%"},
									{"height", "20%"},
									{"centre-x", "50%"},
									{"radio-group", RADIOGROUPNUM},		// Radio group is useless right now. Proper configuration is in a manager.
									{"toggleable", true},
									{"always-on-top", true},
								}
							}
						}
					};
					return axiomSelectaComponent;
				}

				void setup(jive::GuiItem& item) final
				{
					auto* imageComponent = dynamic_cast<juce::Component*>(item.getChildren().getFirst()->getComponent().get());
					jassert(imageComponent != nullptr);
					imageComponent->setInterceptsMouseClicks(false, false);
					
					auto* parentComponent = item.getComponent().get();
					jassert(parentComponent != nullptr);
					parentComponent->setInterceptsMouseClicks(false, true);

					auto* button = dynamic_cast<juce::Button*>(item.getChildren().getLast()->getComponent().get());
					button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
					jassert(button != nullptr);
					button->onClick = [this, button]()
						{
							DBG("AXIOM BUTTON CLICKED!");
							if (button->getToggleState())
							{
								DBG("Button is on.");
								imageSource->set(juce::ImageCache::getFromMemory(BinaryData::AxiomSelectorOn_png,
																				 BinaryData::AxiomSelectorOn_pngSize));
							}
							else
							{
								DBG("Button off.");
								imageSource->set(juce::Image());
							}
						};
					
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