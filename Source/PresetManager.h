#pragma once
#include <JuceHeader.h>
namespace Preset
{
	// All ID's for properties should be stored here.
	namespace Ids
	{
		static const juce::Identifier userRulesetProperty{ "userRulesetNode" };
		static const juce::Identifier userAxiomProperty{ "userAxiomNode" };
		static const juce::Identifier userLsysNameProperty{ "userLsysNameNode" };
		static const juce::Identifier generatedLsysStringProperty{ "generatedLStringNode" };
		static const juce::Identifier notesPoolProperty{ "notesPoolNode" };
		static const juce::Identifier presetNameProperty{ "presetNameProperty" };
	}


	// This class is for managing the backend/component configuration of stuff related to saving and loading presets.
	class PresetManager
		: public juce::Button::Listener, juce::ComboBox::Listener, juce::ValueTree::Listener
	{
	public:
		static const juce::File defaultDirectory;
		static const juce::String presetExtension;

		PresetManager(juce::AudioProcessorValueTreeState& apvts);
		
		void savePreset(const juce::String& presetName);
		void loadPreset(const juce::String& presetName);
		void deletePreset(const juce::String& presetName);

		void configureComboBoxComponent(juce::ComboBox* comboBox);
		void configureSaveButtonComponent(juce::Button* saveButton);

		void buttonClicked(juce::Button* button) override;
		void comboBoxChanged(juce::ComboBox* comboBox) override;
		void valueTreeRedirected(juce::ValueTree& changedValueTree) override;
		void loadPresetList();


	private:
		juce::StringArray getAllPresets() const;

		juce::AudioProcessorValueTreeState& apvts;
		juce::Value currentPreset;

		juce::Button* saveButton;
		juce::ComboBox* comboBox;
		std::unique_ptr<juce::FileChooser> fileChooser;
	};
}