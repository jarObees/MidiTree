#pragma once
#include <JuceHeader.h>
namespace Preset
{


	// This class is for managing the backend/component configuration of stuff related to saving and loading presets.
	// By configuring the comboBox and saveButton component, it's able to work the rest of the magic.

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

		bool isModified; // Will basically only be false at construction, and when preset is loaded/saved.

	private:
		juce::StringArray getAllPresets() const;

		juce::AudioProcessorValueTreeState& apvts;
		juce::Value currentPreset;

		juce::Button* saveButton;
		juce::ComboBox* comboBox;
		std::unique_ptr<juce::FileChooser> fileChooser;
	};
}