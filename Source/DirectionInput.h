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
					bgImage = juce::ImageCache::getFromMemory(BinaryData::NW_OFF_png, BinaryData::NW_ON_pngSize); // Set to base image.
				}

				///TODO: USE A DRAWABLE BUTTON INSTEAD.
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
									new jive::Object
									{
										{"background", jive::toVar(colors::grey)},
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
					if (auto* buttonTing = dynamic_cast<juce::Button*>(item.getComponent().get()))
					{
						buttonTing->setToggleable(true);
						onValueChange = std::make_unique<jive::Event>(item.state, "on-click");
						imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source");
						onValueChange->onTrigger = [this, buttonTing]()
							{
								DBG("Direction Changed! Value: " << (buttonTing->getToggleState() ? "true" : "false"));
								if (buttonTing->getToggleState())
									imageSource->
									set(juce::ImageCache::getFromMemory(BinaryData::IBB_ASCENDING_png, BinaryData::IBB_ASCENDING_pngSize));
								else
									imageSource->
									set(juce::ImageCache::getFromMemory(BinaryData::IBB_DESCENDING_png, BinaryData::IBB_DESCENDING_pngSize));
							};
						onValueChange->trigger();
					}
					else
						jassertfalse;
				}
			private:
				juce::Image bgImage;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
				juce::String id;
				const int rowNum, columnNum;
				std::unique_ptr<jive::Event> onValueChange;
			};
		}
	}
}