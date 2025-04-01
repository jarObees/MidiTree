#pragma once
#include <JuceHeader.h>
namespace Preset
{
	class PresetManager
	{
	public:
		static const juce::File defaultDirectory;
		static const juce::String presetExtension;

		PresetManager(juce::AudioProcessorValueTreeState& apvts);
		void savePreset();
		void loadPreset();
		void deletePreset();

	private:
		juce::AudioProcessorValueTreeState& apvts;
		juce::String currentPreset;
	};
}