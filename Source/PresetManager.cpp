#include "presetManager.h"
namespace Preset
{
	const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(
		juce::File::SpecialLocationType::commonDocumentsDirectory)
		.getChildFile(ProjectInfo::companyName)
		.getChildFile(ProjectInfo::projectName) 
	};
	const juce::String PresetManager::presetExtension{ ".preset" };

	PresetManager::PresetManager(juce::AudioProcessorValueTreeState& _apvts)
		: apvts(_apvts)
	{
		if (!defaultDirectory.exists())
		{
			const auto result = defaultDirectory.createDirectory();
			{
				if (result.failed())
					jassertfalse;
			}
		}
	}

	void PresetManager::savePreset()
	{
		// apvts.copyState()
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