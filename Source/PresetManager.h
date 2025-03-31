#pragma once
#include <JuceHeader.h>
namespace Preset
{
	class PresetManager
	{
	public:
		PresetManager(juce::AudioProcessorValueTreeState& apvts);
		void savePreset();
		void loadPreset();
		void deletePreset();

	private:
		juce::AudioProcessorValueTreeState& apvts;
		juce::String currentPreset;
	};
}