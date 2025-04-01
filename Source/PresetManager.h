#pragma once
#include <JuceHeader.h>
namespace Preset
{
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
		void savePreset();
		void loadPreset();
		void deletePreset();

	private:
		juce::AudioProcessorValueTreeState& apvts;
		juce::String currentPreset;
		juce::String presetName;
	};
}