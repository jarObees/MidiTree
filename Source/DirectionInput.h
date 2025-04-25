#pragma once
namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace DirectionInput
		{
			class DirectionInputView : public jive::View
			{
			public:
				DirectionInputView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = rowColIdMaker(IdPrefix::directionInput, rowNum, columnNum);
				}

				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"Slider",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "25%"},
							{"max", 1},
							{"min", 0},
							{"interval", "1"},
							{"orientation", "horizontal"},
							{"align-content", "centre"},
							{"justify-content", "centre"},
						},
					};
				}
				void setup(jive::GuiItem& item) final
				{
					if (auto* stripSlider = dynamic_cast<juce::Slider*>(item.getComponent().get()))
					{
						// Makes the slider invisible. (Can't just change the alpha, since this affects it's children (the text) as well).
						//stripSlider->setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
						//stripSlider->setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
						//stripSlider->setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack);
						//stripSlider->setOpaque(false);
						onValueChange = std::make_unique<jive::Event>(item.state, "on-change");
						onValueChange->onTrigger = [this, stripSlider]()
							{
								DBG("Dierction Changed! Value: " << stripSlider->getValue());
							};
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