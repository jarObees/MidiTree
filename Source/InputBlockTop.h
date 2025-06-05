#pragma once
#include "Ids.h"

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

			class AxiomSelectorView : public jive::View
			{
			public:
				AxiomSelectorView(int rowNum, int columnNum)
					: rowNum(rowNum), columnNum(columnNum)
				{
					id = rowColIdMaker(IdPrefix::inputBlockAxiom, rowNum, columnNum);
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