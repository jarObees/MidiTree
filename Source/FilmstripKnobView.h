#pragma once

// Args: (juce::Image filmstrip, std::string Id)
// Don't forget to attach the knob to the parameter in the editor if needed!
class FilmstripKnobView : public jive::View
{
public:
	FilmstripKnobView(juce::Image rawTing, std::string rawId)
		: rawFilmstripImage(rawTing), sliderId(rawId)
	{
		sideLength = rawFilmstripImage.getWidth();
		numFrames = rawFilmstripImage.getHeight() / sideLength;
	}

	juce::ValueTree initialise() final
	{
		return juce::ValueTree{
			"Knob",
			{
				{"id", sliderId},
			},
			{
				juce::ValueTree{"Image"},
			},
		};
	}

	void setup(jive::GuiItem& item) final
	{
		// We confirm access to the slider itself.
		if (auto* stripSlider = dynamic_cast<juce::Slider*>(item.getComponent().get()))
		{
			onValueChange = std::make_unique<jive::Event>(item.state, "on-change");
			filmstripSource = std::make_unique<jive::Property<juce::Image>>(item.state.getChild(0), "src"); // Creates the "src" property for the Image value tree(?)
			filmstripSource->set(rawFilmstripImage);
			// When it detects a value change, it displays the appropriate cropped portion of the filmstrip image.
			onValueChange->onTrigger = [this, stripSlider]() {
				std::size_t imageNumber = static_cast<std::size_t>(0.5 + (stripSlider->getValue() - stripSlider->getMinimum()) / (stripSlider->getMaximum() - stripSlider->getMinimum()) * (numFrames - 1));
				juce::Rectangle<int> clippedArea(0, imageNumber * sideLength, sideLength, sideLength);
				// filmstripSource->set(rawFilmstripImage.getClippedImage(clippedArea));
				};
		}
		else
		{
			jassertfalse;
		}
	}

private:
	juce::String sliderId;
	size_t numFrames, sideLength;
	const juce::Image rawFilmstripImage;
	std::unique_ptr<jive::Event> onValueChange;
	std::unique_ptr<jive::Property<juce::Image>> filmstripSource;
};