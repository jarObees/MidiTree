#pragma once
// TODO: Finish implementing juce stuf.
namespace jiveUI
{
	class Button
	{
	public:
		[[nodiscard]] auto& withImage(const juce::Image& rawImage) noexcept
		{
			image = rawImage;
			return *this;
		}
		[[nodiscard]] auto& withId(const juce::String& rawId) noexcept
		{
			id = rawId;
			return *this;
		}
		// Distinguishes between integers and strings.
		// Strings != "0%" as this default value represents if it's uninitialized or not.
		template <typename T>
		[[nodiscard]] auto& withWidth(T rawWidth) noexcept
		{
			if constexpr (std::is_same<T, int>::value)
			{
				width = rawWidth;
			}
			else if constexpr (std::is_same<T, std::string>::value)
			{
				jassert(rawWidth != "0%");
				widthPercentage = rawWidth;
			}
			return *this;
		}
		template <typename T>
		[[nodiscard]] auto& withHeight(T rawHeight) noexcept
		{
			if constexpr (std::is_same<T, int>::value)
			{
				height = rawHeight;
			}
			else if constexpr (std::is_same<T, std::string>::value)
			{
				jassert(rawHeight != "0%");
				heightPercentage = rawHeight;
			}
			return *this;
		}
		[[nodiscard]] auto& withText(std::string rawText) noexcept
		{
			text = rawText;
			return *this;
		}

		[[nodiscard]] auto operator()() const
		{
			auto _width = (widthPercentage == "0%") ? jive::toVar(width) : jive::toVar(widthPercentage);
			auto _height = (heightPercentage == "0%") ? jive::toVar(height) : jive::toVar(heightPercentage);
			return juce::ValueTree
			{
				"Button",
				{
					{"id", id},
					{"width", _width},
					{"height", _height},
					{
						"style",
							new jive::Object
							{
								{ "background", jive::toVar(jiveUI::colors::ground_tertiary)},
							},
					},
				},
			};
		}

	private:
		std::unique_ptr<jive::Event> buttonEvent;
		std::unique_ptr<jive::Property<juce::Image>> buttonSource;
		juce::Image image;
		int width;
		int height;
		juce::String widthPercentage = "0%";
		juce::String heightPercentage = "0%";
		juce::String id;
		juce::String text;
	};
}