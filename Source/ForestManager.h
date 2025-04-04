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
		: public juce::Slider::Listener
	{
	public:
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&);
		void plantTree();
		void configureTreeSlider(juce::Slider* slider);
		void sliderValueChanged(juce::Slider* slider) override;
	
	private:
		juce::AudioProcessorValueTreeState& apvts;
		const int maxNumTrees;
		int currentForestIndex;
		std::vector<std::pair<juce::String, juce::String>> treeContainer; // Vector that should contain the genreatedLString
		juce::Slider* forestSlider = nullptr;
		const Preset::PresetManager& presetManager;
	};
}