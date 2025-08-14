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

	PresetManager::PresetManager(juce::AudioProcessorValueTreeState& _apvts, Tree::ActiveTreeManager& _activeTreeManager)
		: apvts(_apvts), activeTreeManager(_activeTreeManager)
	{
		if (!defaultDirectory.exists())
		{
			const auto result = defaultDirectory.createDirectory();
			{
				if (result.failed())
					jassertfalse;
			}
		}

		// Adds a listener to update the comboBox in realtime.l
		apvts.state.addListener(this);
		isModified = false;
	}

	// Copies state of apvts and savesit to file.
	// Most importantly, used to save/load active tree data.
	// IMPORTANT: Any changes here, insure that everything can be undone using load. Both are intrinsically linked.
	void PresetManager::savePreset(const juce::String& presetName)
		{
		DBG("Saving preset: " << presetName);
		if (presetName.isEmpty())
			return;
		DBG("Previous name: " << activeTreeManager.getTreeName());
		activeTreeManager.setTreeName(presetName);
		DBG("Tree name set. Checking notesPool" << activeTreeManager.getNotesPoolAsNumString());
		std::unique_ptr<juce::XmlElement> xml;

		// In order to put notesPool in xml, we must convert our juce::Array<int> var into a readable xml, since we can't store those for xml.
		// We will then read this back to normal later within the loadPreset().
		if (!activeTreeManager.getNotesPool().isEmpty())
		{
			DBG("fixing the notes pool stuff...");
			auto apvtsCopy = apvts.copyState();
			juce::Array<int> notesPoolArray = activeTreeManager.getNotesPool();
			
			/// FOR DEBUGGING PURPOSES.
			DBG("Confirming notes pool: ");
			for (auto element : notesPoolArray)
			{
				DBG("NOTE: " << juce::String(element));
			}

			// Create a new XML element that holds all the juce::Array data.
			juce::XmlElement notesPoolArrayXML("arrayXml");
			for (auto& noteNum : notesPoolArray) // Converts each int to an xml element.
			{
				auto* noteNumElement = new juce::XmlElement("NoteElement");
				noteNumElement->setAttribute("value", noteNum);
				notesPoolArrayXML.addChildElement(noteNumElement);
			}

			const juce::String serializedNotesPoolArray = notesPoolArrayXML.createDocument("");
			apvtsCopy.setProperty("serializedArray", serializedNotesPoolArray, nullptr);
			apvtsCopy.removeProperty(apvtsPropIds::notesPoolVectorStringProperty, nullptr); // Removes buggy property, once everything else has worked.
			xml = apvtsCopy.createXml();
		}
		else
		{
			DBG("Not fixing notes pool stuff..");
			xml = apvts.copyState().createXml();
		}

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + presetExtension);
		DBG("apvts before saving: " << apvts.state.getProperty("serializedArray").toString());
		if (!xml->writeToFile(presetFile, ""))
		{
			jassertfalse;
		}
		isModified = false;
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
		auto fuckedValueTree = juce::ValueTree::fromXml(*xmlDoc.getDocumentElement());
		juce::String serializedArray = fuckedValueTree.getProperty("serializedArray").toString();
		std::unique_ptr<juce::XmlElement> arrayXml = juce::parseXML(serializedArray);
		jassert(arrayXml != nullptr);
		juce::Array<int> restoredArray;
		for (auto child : arrayXml.get()->getChildIterator())
		{
			if (child->hasTagName("NoteElement"))
			{
				DBG("Adding this num to restoredArray: " << child->getStringAttribute("value").getIntValue());
				restoredArray.add((child->getStringAttribute("value")).getIntValue());
			}
		}
		// fuckedValueTree.setProperty(apvtsPropIds::notesPoolVectorStringProperty, restoredArray, nullptr);
		activeTreeManager.setNotesPool(restoredArray);
		activeTreeManager.setTreeName(presetName);
		Tree::MidiTree newActiveTree{ presetName, restoredArray };
		DBG("Active Tree Name: " << activeTreeManager.getTreeName());
		DBG("Active NotesPool: " << activeTreeManager.getNotesPoolAsNumString());
		apvts.replaceState(fuckedValueTree);
		DBG("Notes Pool is: ");
		auto arrayThing = apvts.state.getProperty(apvtsPropIds::notesPoolVectorStringProperty);
		if (arrayThing.isArray())
		{
			DBG("found da array");
			auto newArray = arrayThing.getArray();
			for (auto element : *newArray)
			{
				DBG("Note: " << element.toString());
			}
		}
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
		activeTreeManager.setTreeName("");
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
		DBG("Button Clicked in Preset Manager: Notes Pool : " << activeTreeManager.getNotesPoolAsNumString());
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

	// TODO: What if user selects the same comboBox?
	// Maybe add a reload button to refresh the current guy.
	void PresetManager::comboBoxChanged(juce::ComboBox* changedComboBox)
	{
		if (changedComboBox == comboBox)
		{
			DBG("Combo Box Change detected.");
			loadPreset(comboBox->getItemText(comboBox->getSelectedItemIndex()));
		}
		// After loading, our presetManager is fresh and set to false. 
		// Have to declare here because reasons beyond my understanding. Don't touch it tho :)
		isModified = false; 
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
		comboBox->setSelectedItemIndex(allPresets.indexOf(activeTreeManager.getTreeName(), juce::dontSendNotification));
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

	// Whenever value tree is copied, this function is called (in this case when we get/set plugin state).
	void PresetManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
	{
		DBG("Value Tree Redirected");
	}
}