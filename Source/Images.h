#pragma once

// Contains the preloaded image assets in the form of a juce::Image.
namespace jiveUI::images
{
    static const juce::Image knobTestWhiteFilmstrip = juce::ImageFileFormat::loadFrom(BinaryData::knobTestWhite_png, BinaryData::knobTestWhite_pngSize);
}