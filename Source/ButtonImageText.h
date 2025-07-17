#pragma once

namespace jiveGui
{
	class DrawableButton : public jive::View
	{
	public:
		// Note: Non-automatable with ->attachToParamater 
		DrawableButton(int rawWidth, int rawHeight, juce::Image rawImage, std::string rawText, juce::String rawId)
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
				},
			};
		}
		std::unique_ptr<juce::Component> createComponent(const juce::ValueTree& tree) final
		{
			if (tree.getType().toString() == "DrawableButton")
				return std::make_unique<juce::DrawableButton>("butTing", juce::DrawableButton::ButtonStyle::ImageRaw);
		}
		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the slider itself.
			if (auto* button = dynamic_cast<juce::DrawableButton*>(item.getComponent().get()))
			{
				button->setImages(mainImage.get());

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