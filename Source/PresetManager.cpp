#include "presetManager.h"
namespace Preset
{
	const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(
		juce::File::SpecialLocationType::userDocumentsDirectory)
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
		const auto xml = apvts.copyState().createXml();
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		//if (!xml->writeToFile(presetFile))
		//{

		//	jassertfalse;
		//}
		currentPreset = presetName;
	}

	void PresetManager::deletePreset()
	{
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		if (!presetFile.existsAsFile())
		{
			DBG("File: " + presetFile.getFullPathName() + " does not exist.");
			jassertfalse;
			return;
		}
		if (!presetFile.deleteFile())
		{
			DBG("File: " + presetFile.getFullPathName() + " could not be deleted.");
			jassertfalse;
			return;
		}
		currentPreset = "";

	}

	void PresetManager::loadPreset()
	{
		// Load a preset from a file.
	}
}