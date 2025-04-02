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

	juce::StringArray PresetManager::getAllPresets() const
	{
		juce::StringArray presets;
		const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, 
															   false, 
															   "*." + presetExtension);
		for (const auto& file : fileArray)
		{
			presets.add(file.getFileNameWithoutExtension());
		}
		return presets;
	}

	void PresetManager::savePreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
			return;
		const auto xml = apvts.copyState().createXml();
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		if (!xml->writeToFile(presetFile, ""))
		{
			jassertfalse;
		}
		currentPreset = presetName;
	}

	void PresetManager::deletePreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
			return;

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

	void PresetManager::loadPreset(const juce::String& presetName)
	{
		if (presetName.isEmpty())
		{
			jassertfalse;
			return;
		}
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		if(!presetFile.existsAsFile())
		{
			jassertfalse;
			return;
		}
		juce::XmlDocument xmlDoc{ presetFile };
		const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDoc.getDocumentElement());
		apvts.replaceState(valueTreeToLoad);
		currentPreset = presetName;
	}

	void PresetManager::configureComboBoxComponent(juce::ComboBox* comboBox)
	{
		//TODO
	}
	void PresetManager::configureSaveButtonComponent(juce::Button* saveButton)
	{
		saveButton->onClick = [this]()
		{
				fileChooser = std::make_unique<juce::FileChooser>
				(
					"Please enter the name of file :3",
					defaultDirectory,
					"*" + presetExtension
				);
				fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [this](const juce::FileChooser& chooser)
										 {
											 const auto resultFile = chooser.getResult();
											 savePreset(resultFile.getFileNameWithoutExtension());
										 });
		};
	}


}