#pragma once

namespace jiveGui
{
	class ButtonImageText : public jive::View
	{
	public:
		ButtonImageText(int rawWidth, int rawHeight, juce::Image rawImage, std::string rawText, juce::String rawId)
			: width(rawWidth), height(rawHeight), imageSource(rawImage), text(rawText), id(rawId)
		{}

		juce::ValueTree initialise() final
		{
			return juce::ValueTree{
				"Button",
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
		void setup(jive::GuiItem& item) final
		{
			// We confirm access to the slider itself.
			if (auto* button = dynamic_cast<juce::Button*>(item.getComponent().get()))
			{
				// Defucks the image component.
				auto* imageComponent = item.getChildren().getFirst()->getComponent().get();
				imageComponent->setInterceptsMouseClicks(false, false);
			}
			else
			{
				jassertfalse;
			}
		}
	private:
		std::unique_ptr<jive::Event> onClick;
		int width;
		int height;
		juce::Image imageSource;
		juce::String id;
		juce::String text;
	};
}