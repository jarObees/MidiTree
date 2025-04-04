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
		juce::AudioProcessorValueTreeState& apvts;
		const int maxNumTrees;
		int currentForestIndex;
		std::vector<std::pair<juce::String, juce::String>> treeContainer; // Vector that should contain the genreatedLString
		
		juce::Slider* forestSlider = nullptr;
		juce::Button* forestButton = nullptr;

		const Preset::PresetManager& presetManager;

		void plantTree();
	};
}