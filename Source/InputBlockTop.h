#pragma once
#include "Ids.h"
#include "Colors.h"

namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace InputBlockTop
		{
			class OctavesInputView : public jive::View
			{
			public:
				OctavesInputView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = idRowColMaker(IdPrefix::octavesInput, rowNum, columnNum);
					bgImage = juce::ImageCache::getFromMemory(BinaryData::OCTAVES_0_png, BinaryData::OCTAVES_0_pngSize); // Set to base image.
				}
				
				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"Slider",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "25%"},
							{"orientation", "vertical"},
							{"align-content", "centre"},
							{"justify-content", "centre"},
							{"flex-grow", 1},
							{
								"style",
									new jive::Object{
										{"background", jive::toVar(colors::darkGray)},
									}
							}
						},
						{
							juce::ValueTree{
								"Image",
								{
									{"source", jive::toVar(bgImage)},
								}
							},
						},
					};
				}
				void setup(jive::GuiItem& item) final
				{
					if (auto* stripSlider = dynamic_cast<juce::Slider*>(item.getComponent().get()))
					{
						juce::NormalisableRange<double> range(0.0f, 2.0f, 1.0f);
						stripSlider->setNormalisableRange(range);
						onValueChange = std::make_unique<jive::Event>(item.state, "on-change");
						imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source");
						onValueChange->onTrigger = [this, stripSlider]()
							{
								DBG("Octave Input Changed! Value: " << stripSlider->getValue());
								switch (int(stripSlider->getValue()))
								{
								case 0:
									DBG("Case 0");
									imageSource->
										set(juce::ImageCache::getFromMemory(BinaryData::OCTAVES_0_png, BinaryData::OCTAVES_0_pngSize));
									break;
								case 1:
									DBG("Case 1");
									imageSource->
										set(juce::ImageCache::getFromMemory(BinaryData::OCTAVES_1_png, BinaryData::OCTAVES_1_pngSize));
									break;
								case 2:
									DBG("Case 2");
									imageSource->
										set(juce::ImageCache::getFromMemory(BinaryData::OCTAVES_3_png, BinaryData::OCTAVES_3_pngSize));
									break;
								}
							};
						stripSlider->setValue(0); // Set default value to 0.
						onValueChange->trigger();
					}
					else
						jassertfalse;
				}
			private:
				juce::Image bgImage;
				juce::String id;
				const int rowNum, columnNum;
				std::unique_ptr<jive::Event> onValueChange;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
			};

			// IF WE HAVE AN AXIOM CLASS =============================================================================================
			class AxiomSelectorView : public jive::View
			{
			public:
				AxiomSelectorView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = idRowColMaker(IdPrefix::inputBlockAxiom, rowNum, columnNum);
				}

				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"Button",
						{
							{"id", id},
							{"toggleable", true},
							{"radio-group", 1},
							{"width", "100%"},
							{"height", "25%"},
							{"orientation", "vertical"},
							{"align-content", "centre"},
							{"justify-content", "centre"},
							{"flex-grow", 1},
												{
					"style",
						new jive::Object{
							{"background", jive::toVar(colors::red)},
						}
					}
						},
					};
				}

				void setup(jive::GuiItem& item) final
				{
					if (auto* axiomButton = dynamic_cast<juce::Button*>(item.getComponent().get()))
					{
						onValueChange = std::make_unique<jive::Event>(item.state, "on-click");
						onValueChange->onTrigger = [this, axiomButton]()
							{
								DBG("Axiom Input toggled! State: " << axiomButton->getState());
							};
						onValueChange->trigger();
					}
					else
						jassertfalse;
				}

			private:
				juce::String id;
				const int rowNum, columnNum;
				std::unique_ptr<jive::Event> onValueChange;
			};
		}
	}
}