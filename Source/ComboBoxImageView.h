#pragma once

// Args: (int width, int height, juce::StringArray comboBoxItems, std::string comboBoxId)
namespace jiveGui
{
	class ImageComboBoxView : public jive::View, juce::MouseListener 
	{
	public:
		ImageComboBoxView(int rawWidth, 
						  int rawHeight, 
						  juce::String rawComboBoxId, 
						  juce::Image _normalImage, 
						  std::vector<juce::Image> _dynamicImageOptions = std::vector<juce::Image>{},
						  juce::Image _overImage = juce::Image{}, 
						  juce::Image _downImage = juce::Image{})
			: width(rawWidth), 
			height(rawHeight), 
			comboBoxId(rawComboBoxId), 
			baseImage(_normalImage), 
			mouseOverImage(_overImage), 
			mouseDownImage(_downImage),
			dynamicImageOptions(_dynamicImageOptions)
		{
		}

		juce::ValueTree initialise() final
		{
			return juce::ValueTree{
				"Component",
				{
					{"display", "block"},
					{"width", width},
					{"height", height},
									{
					"style",
						new jive::Object{
							{ "background", jive::toVar(colors::sky_primary)},
						},
				},
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
							{"source", jive::toVar(baseImage)},
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
				///TODO: FIX THING BEING BLANK IF NOTHING IS SELECTED. WE SHOULDN'T REALLY HAVE A BASE IMAGE IN SOME CASES (AKA SOMETHING IS ALWAHYS SELECTED).
				comboBox->setMouseCursor(juce::MouseCursor::PointingHandCursor);
				comboBox->setVisible(true);
				comboBox->addMouseListener(this, true);
				onValueChange = std::make_unique<jive::Event>(comboBoxGuiItem->state, "on-change");
				imageSource = std::make_unique<jive::Property<juce::Image>>(item.getChildren().getLast()->state, "source");
				if (comboBox->getSelectedId() > 0 && comboBox->getSelectedId() <= dynamicImageOptions.size())
					uniqueItemImage = dynamicImageOptions[comboBox->getSelectedId()];
				onValueChange->onTrigger = [this, comboBox]()
				{
						int itemId = comboBox->getSelectedId();
						DBG("NoteType Image set to " << itemId);

						// Ensure image/selection match.
						if (itemId > 0 && itemId <= dynamicImageOptions.size())
						{
							DBG("Image selected.");
							jassert(dynamicImageOptions[itemId].isValid());
							uniqueItemImage = dynamicImageOptions[itemId];
							imageSource->set(uniqueItemImage);
						}
				};
				onValueChange->trigger();
			}
			else
			{
				jassertfalse;
			}
		}

	private:
		// For visual updates.
		void mouseEnter(const juce::MouseEvent& event) override
		{
			DBG("Mouse enter combo box");
			if (mouseOverImage.isValid())
				imageSource->set(mouseOverImage);
		}
		void mouseDown(const juce::MouseEvent& event) override
		{
			DBG("Mouse down on combo box");
			if (mouseDownImage.isValid())
				imageSource->set(mouseDownImage);
		}
		void mouseExit(const juce::MouseEvent& event) override
		{
			DBG("mouse exit combo box");
			if (dynamicImageOptions.size() > 0)
				imageSource->set(uniqueItemImage);
			else
				imageSource->set(baseImage);
		}

		std::unique_ptr<jive::Event> onValueChange;
		std::unique_ptr<jive::Property<juce::Image>> imageSource;
		juce::Image uniqueItemImage;
		juce::Image baseImage;
		juce::Image mouseOverImage;
		juce::Image mouseDownImage;
		std::vector<juce::Image> dynamicImageOptions;	// Contains a vector that has images corresponding to item ID. 
		int width, height;
		const juce::String comboBoxId;
	};
}