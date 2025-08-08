#pragma once

namespace jiveGui
{
	class DrawableButton : public jive::View
	{
	public:
		// Note: Non-automatable with ->attachToParamater 
		DrawableButton(int rawWidth, 
					   int rawHeight, 
					   juce::Image rawImage, 
					   juce::String rawId, 
					   juce::Image _hoverImage = juce::Image{}, 
					   juce::Image _pressedImage = juce::Image{})
			: width(rawWidth), height(rawHeight), imageSource(rawImage), id(rawId)
		{
			mainImage = std::make_unique<juce::DrawableImage>(imageSource);
			hoverImage = std::make_unique<juce::DrawableImage>(_hoverImage);
			pressedImage = std::make_unique<juce::DrawableImage>(_pressedImage);
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
				button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
				button->setImages(mainImage.get(), hoverImage.get(), pressedImage.get());
				button->onClick = [this, button]()
					{
						DBG(button->getComponentID() << " was clicked.");
					};
			}
			else
			{
				jassertfalse;
			}
		}
	private:
		std::unique_ptr<juce::DrawableImage> mainImage;
		std::unique_ptr<juce::DrawableImage> hoverImage;
		std::unique_ptr<juce::DrawableImage> pressedImage;
		std::unique_ptr<jive::Event> onClick;
		int width;
		int height;
		juce::Image imageSource;
		juce::String id;
	};
}