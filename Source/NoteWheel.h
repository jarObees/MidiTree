#pragma once
#include "Colors.h"

namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace NoteWheel
		{
			static const inline std::vector intervals{ "-", "1", "b2", "2", "b3", "3", "4", "b5", "5", "b6", "6", "b7", "7" };
			
			class NoteWheelView
				: public jive::View
			{
			public:
				NoteWheelView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = rowColIdMaker(IdPrefix::noteWheel, rowNum, columnNum);
					bgImage = juce::ImageCache::getFromMemory(BinaryData::NW_OFF_png, BinaryData::NW_ON_pngSize); // Set to base image.
				}

				juce::ValueTree initialise() final
				{
					return juce::ValueTree{
						"Component",
						{
							{"display", "block"},
							{"width", "100%"},
							{"height", "50%"},
						},
						{
							juce::ValueTree
							{
								"Slider",
								{
									{"id", id},
									{"width", "100%"},
									{"height", "100%"},
									{"max", 12},
									{"min", 0},
									{"interval", "1"},
									{"orientation", "vertical"},
									{"align-content", "centre"},
									{"justify-content", "centre"},
								},
								{
									juce::ValueTree
									{
										"Image",
										{
											{"source", jive::toVar(bgImage)},
											{"width", "100%"},
											{"height", "100%"},
										},
									},
								},
							},
							juce::ValueTree
							{
								"Text",
								{
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
												{ "foreground", "#000000"},
												{ "font-size", 20 },
												{ "letter-spacing", 1 },
											},
									},
								},
							},
						},
					};
				}

				void setup(jive::GuiItem& item) final
				{
					onValueChange = std::make_unique<jive::Event>(item.getChildren().getFirst()->state, "on-change");
					imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0).getChild(0), "source");
					textProperty = std::make_unique<jive::Property<juce::String>>(item.state.getChild(1), "text");
					jassert(imageSource != nullptr);
					jassert(onValueChange != nullptr);

					auto* stripSlider = dynamic_cast<juce::Slider*>(item.getChildren().getFirst()->getComponent().get());
					auto* textLabelThing = item.getChildren().getLast()->getComponent().get();
					textLabelThing->setInterceptsMouseClicks(false, false);
					jassert(textLabelThing != nullptr);
					jassert(stripSlider != nullptr);

					// Makes the slider invisible. (Can't just change the alpha, since this affects it's children (the text) as well).
					stripSlider->setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
					stripSlider->setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
					stripSlider->setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack);
					onValueChange->onTrigger = [this, stripSlider]()
					{
						DBG("Note Wheel changed! Value: " << stripSlider->getValue());
						int index = static_cast<int>(stripSlider->getValue());
						DBG("Setting text to: " << intervals[index]);
						textProperty->set(intervals[index]);
						if (stripSlider->getValue() == 0)
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NW_OFF_png, BinaryData::NW_OFF_pngSize));
						else
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NW_ON_png, BinaryData::NW_ON_pngSize));
					};
					//stripSlider->setValue(0); // Set default value to 0.
					onValueChange->trigger();
				}

			private:
				juce::String id;
				const int rowNum, columnNum;
				juce::String text;
				juce::Image bgImage;

				std::unique_ptr<jive::Event> onValueChange;
				std::unique_ptr<jive::Property<juce::String>> textProperty;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
			};
		}
	}
}
