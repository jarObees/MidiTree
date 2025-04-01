#pragma once
#include <JuceHeader.h>
namespace Preset
{
	// All ID's for propertys should be stored here.
	namespace Ids
	{
		static const juce::Identifier userRulesetProperty{ "userRulesetNode" };
		static const juce::Identifier userAxiomProperty{ "userAxiomNode" };
		static const juce::Identifier userLsysNameProperty{ "userLsysNameNode" };
		static const juce::Identifier generatedLsysStringProperty{ "generatedLStringNode" };
		static const juce::Identifier notesPoolProperty{ "notesPoolNode" };
	}

	class PresetManager
	{
	public:
		static const juce::File defaultDirectory;
		static const juce::String presetExtension;

		PresetManager(juce::AudioProcessorValueTreeState& apvts);
		void savePreset(const juce::String& presetName);
		void loadPreset();
		void deletePreset(const juce::String& presetName);
		
		void configureComboBoxComponent(juce::ComboBox* comboBox);
		void configureSaveButtonComponent(juce::Button* saveButton);

	private:
		juce::AudioProcessorValueTreeState& apvts;
		juce::String currentPreset;

		juce::Button* saveButton;
		std::unique_ptr<juce::FileChooser> fileChooser;
	};
}