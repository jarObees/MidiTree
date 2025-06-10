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
						"Button",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "25%"},
							{"toggled", false },
							{"toggleable", true},
							{"toggle-on-click", true},
							{"orientation", "horizontal"},
							{"align-content", "centre"},
							{"justify-content", "centre"},
												{
					"style",
						new jive::Object{
							{"background", jive::toVar(colors::grey)},
						}
					}
						},
					};
				}
				void setup(jive::GuiItem& item) final
				{
					if (auto* buttonTing = dynamic_cast<juce::Button*>(item.getComponent().get()))
					{
						buttonTing->setToggleable(true);
						onValueChange = std::make_unique<jive::Event>(item.state, "on-click");
						onValueChange->onTrigger = [this, buttonTing]()
							{
								DBG("Direction Changed! Value: " << (buttonTing->getToggleState() ? "true" : "false"));
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