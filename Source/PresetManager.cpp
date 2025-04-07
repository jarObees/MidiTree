#include "presetManager.h"
#include "Ids.h"

namespace Preset
{
	const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(
		juce::File::SpecialLocationType::userDocumentsDirectory)
		.getChildFile(ProjectInfo::companyName)
		.getChildFile(ProjectInfo::projectName) 
	};
	const juce::String PresetManager::presetExtension{ "preset" };

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

		// Adds a listener to update the comboBox in realtime.
		apvts.state.addListener(this);
		currentPreset.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
		DBG("RULESET IN PRESET MANAGER CONSTRUCTION" << (apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr)));
		DBG("Set the current preset");
	}

	// Copies state of apvts and savesit to file.
	void PresetManager::savePreset(const juce::String& presetName)
	{
		DBG("Saving preset: " << presetName);
		if (presetName.isEmpty())
			return;

		currentPreset.setValue(presetName);
		
		
		std::unique_ptr<juce::XmlElement> xml;

		// In order to put into xml, we must convert our juce::Array<int> var into a readable xml, since we can't store those for xml.
		// We will then read this back to normal later within the loadPreset().
		if (!apvts.state.getProperty(apvtsPropIds::notesPoolVectorStringProperty).isVoid())
		{
			DBG("fixing the notes pool stuff...");
			auto apvtsCopy = apvts.copyState();
			juce::Array<juce::var> notesPoolArray = *apvtsCopy.getProperty(apvtsPropIds::notesPoolVectorStringProperty).getArray();

			// Create a new XML element that holds all the juce::Array data.
			juce::XmlElement arrayXml("arrayXml");
			for (auto& item : notesPoolArray) // Grabs each var (which is an int) from the notes pool, and converts it to an xml element.
			{
				jassert(item.isInt());
				int item = item;
				auto* element = new juce::XmlElement("NoteElement");
				element->setAttribute("value", item);
				arrayXml.addChildElement(element);
			}

			const juce::String serializedArray = arrayXml.createDocument("");
			apvtsCopy.setProperty("serializedArray", serializedArray, nullptr);
			apvtsCopy.removeProperty(apvtsPropIds::notesPoolVectorStringProperty, nullptr); // Removes buggy property, once everything else has worked.
			xml = apvtsCopy.createXml();
		}
		else
		{
			DBG("Not fixing notes pool stuff..");
			xml = apvts.copyState().createXml();
		}

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		if (!xml->writeToFile(presetFile, ""))
		{
			jassertfalse;
		}

	}

	// After passing the preset name, loads in said preset to the apvts. 
	void PresetManager::loadPreset(const juce::String& presetName)
	{
		DBG("Loading preset: " << presetName);
		if (presetName.isEmpty())
		{
			jassertfalse;	
			return;
		}
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		if (!presetFile.existsAsFile())
		{
			jassertfalse;
			return;
		}
		juce::XmlDocument xmlDoc{ presetFile };

		// DEFUCKER
		//TODO: Double check that this thing is working how I want it to work.
		auto fuckedValueTree = juce::ValueTree::fromXml(*xmlDoc.getDocumentElement());
		juce::String serializedArray = fuckedValueTree.getProperty("serializedArray").toString();
		std::unique_ptr<juce::XmlElement> arrayXml = juce::parseXML(serializedArray);
		jassert(arrayXml != nullptr);
		juce::Array<juce::var> restoredArray;
		for (auto child : arrayXml.get()->getChildIterator())
		{
			if (child->hasTagName("NoteElement"))
			{
				auto thing = child->getStringAttribute("value");
				restoredArray.add((child->getStringAttribute("value")).getIntValue());
			}
		}
		fuckedValueTree.setProperty(apvtsPropIds::notesPoolVectorStringProperty, restoredArray, nullptr);

		apvts.replaceState(fuckedValueTree);
		DBG("NOTES POOL IS: " << apvts.state.getProperty(apvtsPropIds::notesPoolVectorStringProperty));
		currentPreset.setValue(presetName);
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
		currentPreset.setValue("");
	}

	void PresetManager::configureComboBoxComponent(juce::ComboBox* _comboBox)
	{
		DBG("Configuring ComboBox");
		comboBox = _comboBox;
		jassert(comboBox != nullptr);
		comboBox->setTextWhenNothingSelected("No Tree Selected :(");
		comboBox->setMouseCursor(juce::MouseCursor::PointingHandCursor);
		comboBox->addListener(this);
		loadPresetList();

	}

	void PresetManager::configureSaveButtonComponent(juce::Button* _saveButton)
	{
		saveButton = _saveButton;
		saveButton->addListener(this);
	}

	// General buttonClicked override for any related button.
	void PresetManager::buttonClicked(juce::Button* button)
	{
		DBG("Save button clicked");
		
		// Opens up a file picker window for the user to save.
		if (button == saveButton)
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
										 DBG("Passing preset name to saveFile Function as: " << resultFile.getFileNameWithoutExtension());
										 savePreset(resultFile.getFileNameWithoutExtension());
										 loadPresetList(); // Reloads preset list right after saving to refresh it.
									 });
		}
	}

	// Geenral comboBoxChanged for any related button.
	void PresetManager::comboBoxChanged(juce::ComboBox* changedComboBox)
	{
		if (changedComboBox == comboBox)
		{
			DBG("Combo Box Change detected.");
			loadPreset(comboBox->getItemText(comboBox->getSelectedItemIndex()));
		}
	}

	// Refreshes preset list on comboBox component with the latest presets.
	void PresetManager::loadPresetList()
	{
		if (comboBox == nullptr)
		{
			jassertfalse;
			return;
		}

		comboBox->clear(juce::dontSendNotification);
		const auto allPresets = getAllPresets();
		comboBox->addItemList(allPresets, 1);
		comboBox->setSelectedItemIndex(allPresets.indexOf(currentPreset.toString()), juce::dontSendNotification);
	}

	// Helper to loadPresetList()
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

	// Whenever value tree is copied, this function isc alled.
	void PresetManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
	{
		DBG("Value Tree Redirected");
		currentPreset.referTo(changedValueTree.getPropertyAsValue(apvtsPropIds::presetNameProperty, nullptr));
	}
}