#pragma once

// Contains the preloaded image assets in the form of a juce::Image.
namespace jiveUI::images
{
    static const juce::Image knobTestWhiteFilmstrip = juce::ImageFileFormat::loadFrom(BinaryData::knobTestWhite_png, BinaryData::knobTestWhite_pngSize);
    static const juce::Image horiSliderWhiteFilmstrip = juce::ImageFileFormat::loadFrom(BinaryData::_128_HORISLIDER_FILMSTRIP_png, BinaryData::_128_HORISLIDER_FILMSTRIP_pngSize);
}