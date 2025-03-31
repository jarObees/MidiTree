#include "presetManager.h"
namespace Preset
{
	PresetManager::PresetManager(juce::AudioProcessorValueTreeState& _apvts)
		: apvts(_apvts)
	{
	}
	void PresetManager::savePreset()
	{
		// Save the current state of the plugin to a file.
	}
	void PresetManager::loadPreset()
	{
		// Load a preset from a file.
	}
	void PresetManager::deletePreset()
	{
		// Delete a preset from the list of presets.
	}
}