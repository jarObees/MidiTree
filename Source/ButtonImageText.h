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
			: width(rawWidth), height(rawHeight), imageSource(rawImage), id(rawId),
			hoverImageSource(_hoverImage), pressedImageSource(_pressedImage)
		{
			mainImage = std::make_unique<juce::DrawableImage>(imageSource);
			if (_hoverImage.isNull())
				hoverImage.reset();
			else
				hoverImage = std::make_unique<juce::DrawableImage>(hoverImageSource);

			if (_pressedImage.isNull())
				pressedImage.reset();
			else
				pressedImage = std::make_unique<juce::DrawableImage>(pressedImageSource);
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
				button->setImages(mainImage.get(), hoverImage.get(), pressedImage.get()); ///TODO: Figure out why it disappears.
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
		juce::Image hoverImageSource;
		juce::Image pressedImageSource;
		juce::String id;
	};
}