#pragma once

// Args: (int width, int height, juce::StringArray comboBoxItems, std::string comboBoxId)
namespace jiveGui
{
	class ImageComboBoxView : public jive::View
	{
	public:
		ImageComboBoxView(int rawWidth, int rawHeight, juce::String rawComboBoxId)
			: width(rawWidth), height(rawHeight), comboBoxId(rawComboBoxId)
		{
			imageToShow = juce::ImageCache::getFromMemory(BinaryData::NoteSelectorBase_png, BinaryData::NoteSelectorBase_pngSize);
		}

		juce::ValueTree initialise() final
		{
			return juce::ValueTree{
				"Component",
				{
					{"display", "block"},
					{"width", width},
					{"height", height},
				},
				{
					juce::ValueTree
					{
						"ComboBox",
						{
							{"id", comboBoxId},
							{"always-on-top", "true"},
							{"width", "100%"},
							{"height","100%"},
						},
					},
					juce::ValueTree
					{
						"Image",
						{
							{"width", "100%"},
							{"height", "100%"},
							{"source", jive::toVar(imageToShow)},
						}
					}
				}
			};
		}

		// Only do visual stuff here.
		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the comboBox itself.
			auto comboBoxGuiItem = item.getChildren().getFirst();
			if (auto* comboBox = dynamic_cast<juce::ComboBox*>(comboBoxGuiItem->getComponent().get()))
			{
				comboBox->setVisible(true);
				onValueChange = std::make_unique<jive::Event>(comboBoxGuiItem->state, "on-change");
				imageSource = std::make_unique<jive::Property<juce::Image>>(item.getChildren().getLast()->state, "source");
				onValueChange->onTrigger = [this, comboBox]()
				{
						int itemId = comboBox->getSelectedId();
						DBG("NoteType Image set to " << itemId);
						switch (itemId)
						{
						case 0:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorBase_png, BinaryData::NoteSelectorBase_pngSize));
							break;
						case 1:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorQuarter_png, BinaryData::NoteSelectorQuarter_pngSize));
							break;
						case 2:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorDotted_png, BinaryData::NoteSelectorDotted_pngSize));
							break;
						case 3:
							imageSource->set(juce::ImageCache::getFromMemory(BinaryData::NoteSelectorTriplet_png, BinaryData::NoteSelectorTriplet_pngSize));
							break;
						}
				};
			}
			else
			{
				jassertfalse;
			}
		}

	private:
		std::unique_ptr<jive::Event> onValueChange;
		std::unique_ptr<jive::Property<juce::Image>> imageSource;
		juce::Image imageToShow;
		int width, height;
		const juce::String comboBoxId;
	};
}