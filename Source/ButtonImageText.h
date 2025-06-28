#pragma once

namespace jiveGui
{
	class ButtonImageText : public jive::View
	{
	public:
		ButtonImageText(int rawWidth, int rawHeight, juce::Image rawImage, std::string rawText, juce::String rawId)
			: width(rawWidth), height(rawHeight), imageSource(rawImage), text(rawText), id(rawId)
		{
			mainImage = std::make_unique<juce::DrawableImage>(imageSource);
		}

		juce::ValueTree initialise() final
		{
			return juce::ValueTree{
				"DrawableButton",
				{
					{"id", id},
					{"width", width},
					{"height", height},
					{"justify-content", "centre"},
					{
						"style",
							new jive::Object
							{
								{ "background", jive::toVar(jiveGui::colors::ground_tertiary)},
							},
					},
				},
				{
					juce::ValueTree{
						"Text",
						{
							{"text", jive::toVar(text)},
						}
					},
				},
			};
		}
		std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
		{
			if (tree.getType().toString() == "DrawableButton")
				return std::make_unique<juce::DrawableButton>("butTing", juce::DrawableButton::ButtonStyle::ImageRaw);
			else
			{
				return nullptr;
			}
		}
		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the slider itself.
			if (auto* button = dynamic_cast<juce::DrawableButton*>(item.getComponent().get()))
			{
				// Defucks the image component.
				auto* imageComponent = item.getChildren().getFirst()->getComponent().get();
				imageComponent->setInterceptsMouseClicks(false, false);

				button->setImages(mainImage.get()); // But if I use this, I get THE error.

			}
			else
			{
				jassertfalse;
			}
		}
	private:
		std::unique_ptr<juce::DrawableImage> mainImage;
		std::unique_ptr<jive::Event> onClick;
		int width;
		int height;
		juce::Image imageSource;
		juce::String id;
		juce::String text;
	};
}