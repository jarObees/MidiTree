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

				std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
				{
					if (tree.getType().toString() == "DrawableButton")
						return std::make_unique<juce::DrawableButton>("butTing2", juce::DrawableButton::ButtonStyle::ImageRaw);
				}

				juce::ValueTree initialise()
				{
					return juce::ValueTree{
						"DrawableButton",
						{
							{"id", id},
							{"width", "100%"},
							{"height", "25%"},
							{"align-content", "centre"},
							{"justify-content", "centre"},
						},
					};
				}
				void setup(jive::GuiItem& item) final
				{
					if (auto* buttonTing = dynamic_cast<juce::DrawableButton*>(item.getComponent().get()))
					{
						offImage = std::make_unique<juce::DrawableImage>
							(juce::ImageCache::getFromMemory(BinaryData::IBB_DESCENDING_png, 
															 BinaryData::IBB_DESCENDING_pngSize));
						onImage = std::make_unique<juce::DrawableImage>
							(juce::ImageCache::getFromMemory(BinaryData::IBB_ASCENDING_png,
															 BinaryData::IBB_ASCENDING_pngSize));
						buttonTing->setImages(onImage.get(), nullptr, nullptr, nullptr, offImage.get());
						buttonTing->setToggleable(true);
						buttonTing->setClickingTogglesState(true);
					}
					else
						jassertfalse;
				}
			private:
				std::unique_ptr<juce::DrawableImage> onImage;
				std::unique_ptr<juce::DrawableImage> offImage;
				juce::String id;
				const int rowNum, columnNum;
				std::unique_ptr<jive::Event> onValueChange;
			};
		}
	}
}