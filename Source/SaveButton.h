#pragma once

namespace jive_gui
{
	class SaveButton : public jive::View
	{
	public:
		SaveButton(int rawWidth, int rawHeight, juce::Image rawImage, std::string rawId)
			: width(rawWidth), height(rawHeight), imageSource(rawImage), id(rawId) 
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
								{ "background", jive::toVar(jive_gui::colors::ground_tertiary)},
							},
					},
				},
				{
					juce::ValueTree{
						"Text",
						{
							{"text", "save"},
						}
					},
				},
				//{
				//	juce::ValueTree{
				//		"Image",
				//		{
				//			{"source", jive::toVar(imageSource)},
				//		}
				//	},
				//},
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
	};
}