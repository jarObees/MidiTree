#pragma once

namespace jiveGui
{
	class DrawableToggleButton : public jive::View
	{
	public:
		// Note: Non-automatable with ->attachToParamater 
		DrawableToggleButton(int rawWidth, int rawHeight, juce::Image onImage, juce::Image offImage, juce::String rawId, bool clickable = true)
			: width(rawWidth), height(rawHeight), onImageSource(onImage), offImageSource(offImage), id(rawId), isClickable(clickable)
		{
			onImageDrawable = std::make_unique<juce::DrawableImage>(onImageSource);
			offImageDrawable = std::make_unique<juce::DrawableImage>(offImageSource);
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
				button->setImages(offImageDrawable.get(), nullptr, nullptr, nullptr, onImageDrawable.get());
				button->setToggleable(true);
				if (isClickable)
				{
					button->setClickingTogglesState(true);
					button->setMouseCursor(juce::MouseCursor::PointingHandCursor);
					button->onClick = [this, button]()
						{
							DBG("Toggle Button Clicked.");
							DBG("Current toggle state: " << (button->getToggleState() ? "true" : "false"));
						};
				}
				else
				{
					button->setClickingTogglesState(false);
					button->setMouseCursor(juce::MouseCursor::NormalCursor);
				}
			}
			else
			{
				jassertfalse;
			}
		}
	private:
		bool isClickable;
		std::unique_ptr<juce::DrawableImage> onImageDrawable;
		std::unique_ptr <juce::DrawableImage> offImageDrawable;
		int width;
		int height;
		juce::Image onImageSource;
		juce::Image offImageSource;
		juce::String id;
		juce::String text;
	};
}