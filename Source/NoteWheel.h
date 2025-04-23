#pragma once

namespace jive_gui
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
				juce::ValueTree initialise() final
				{
					return juce::ValueTree{
						"Slider",
						{
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
								"Text",
								{
									{"text", jive::toVar(text)},
									{"justification", juce::Justification::centred},
									{
									"style",
										new jive::Object
											{
												{ "foreground", "#000000"},
												{ "font-size", 20 },
												{ "letter-spacing", 1 },
											},
									},
								},
							}
						}
					};
				}

				void setup(jive::GuiItem& item) final
				{
					if (auto* stripSlider = dynamic_cast<juce::Slider*>(item.getComponent().get()))
					{
						auto* textLabelThing = item.getChildren().getFirst()->getComponent().get();
						textLabelThing->setInterceptsMouseClicks(false, false);
						// Ja
						stripSlider->setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
						stripSlider->setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
						stripSlider->setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack);
						stripSlider->setOpaque(false);
						// Configure text if needed.
						onValueChange = std::make_unique<jive::Event>(item.state, "on-change");
						textProperty = std::make_unique<jive::Property<juce::String>>(item.state.getChild(0), "text");
						onValueChange->onTrigger = [this, stripSlider]()
						{
							DBG("Value changed!");
							int index = static_cast<int>(stripSlider->getValue());
							textProperty->set(intervals[index]);
						};
					}
					else
						jassertfalse;
				}

			private:
				juce::String text;
				std::unique_ptr<jive::Event> onValueChange;
				std::unique_ptr<jive::Property<juce::String>> textProperty;
			};
		}
	}
}
