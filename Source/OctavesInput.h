#pragma once
#include "Ids.h"

namespace jiveGui
{
	namespace AnalogUserInput
	{
		namespace OctaveInput
		{
			class OctavesInputView : public jive::View
			{
			public:
				OctavesInputView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = rowColIdMaker(IdPrefix::octavesInput, rowNum, columnNum);
				}
				
				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"Slider",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "25%"},
							{"max", 2},
							{"min", 0},
							{"interval", "1"},
							{"orientation", "vertical"},
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
								DBG("Octave Input Changed! Value: " << stripSlider->getValue());
							};
						stripSlider->setValue(0); // Set default value to 0.
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