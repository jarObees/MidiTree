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
					id = idRowColMaker(IdPrefix::directionInput, rowNum, columnNum);
					baseImage = juce::ImageCache::getFromMemory(BinaryData::IBB_ASCENDING_png, BinaryData::IBB_ASCENDING_pngSize);
				}

				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"Component",
						{
							{"width", "100%"},
							{"height", "25%"},
							{"display", "block"},
						},
						{
							juce::ValueTree
							{
								"Image",
								{
									{"width", "100%"},
									{"height", "100%"},
									{"source", jive::toVar(baseImage)},
								}
							},
							juce::ValueTree
							{
								"Button",
								{
									{"id", id},
									{"width", "100%"},
									{"height", "100%"},
								},
							},
						}
					};
				}

				void setup(jive::GuiItem& item) final
				{

					imageSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "source");
					jassert(imageSource != nullptr);

					if (auto* buttonTing = dynamic_cast<juce::Button*>(item.getChildren().getLast()->getComponent().get()))
					{
						buttonTing->setToggleable(true);
						buttonTing->setClickingTogglesState(true);
						buttonTing->setMouseCursor(juce::MouseCursor::PointingHandCursor);
						buttonTing->onClick = [this, buttonTing]()
							{
								setImage(buttonTing);
							};
						setImage(buttonTing);
					}
					else
						jassertfalse;
				}
			private:
				void setImage(juce::Button* button)
				{
					DBG("Click detected!");
					if (button->getToggleState())
					{
						imageSource->set(juce::ImageCache::getFromMemory(BinaryData::IBB_DESCENDING_png, BinaryData::IBB_DESCENDING_pngSize)); // This doesn't work.
					}
					else
					{
						imageSource->set(juce::ImageCache::getFromMemory(BinaryData::IBB_ASCENDING_png, BinaryData::IBB_ASCENDING_pngSize));
					}
				}
				juce::Image baseImage;
				std::unique_ptr<jive::Property<juce::Image>> imageSource;
				juce::String id;
				const int rowNum, columnNum;
				std::unique_ptr<jive::Event> onValueChange;
			};
		}
	}
}