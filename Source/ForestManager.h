#pragma once
#include <JuceHeader.h>

namespace Preset { class PresetManager; }

namespace Forest
{
	namespace midiTreePair
	{
		static const juce::String lStringToShow = "lstringtoshow";
		static const juce::String lStringToComputer = "lstringtocomputer";
	}

	class ForestManager
		: public juce::Slider::Listener, juce::Button::Listener
	{
	public:
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&);
		void configureTreeSlider(juce::Slider* slider);
		void configureForestButton(juce::Button* button);
		
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
	
	private:
		juce::Value generatedLString;
		juce::Value notesPool;

		juce::AudioProcessorValueTreeState& apvts;
		const int maxNumTrees;
		int currentForestIndex;
		std::vector<std::pair<juce::String, juce::Array<int>>> forestDataSlots; // Slider will be indexing across this vector.
		
		juce::Slider* forestSlider = nullptr;
		juce::Button* forestButton = nullptr;

		const Preset::PresetManager& presetManager;

		void plantTree();
	};
}