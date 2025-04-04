#pragma once
#include <JuceHeader.h>

namespace Preset { class PresetManager; }

namespace Forest
{
	class ForestManager
		: public juce::Slider::Listener
	{
	public:
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&);
		void plantTree();
		void configureTreeSlider(juce::Slider* slider);
		void sliderValueChanged(juce::Slider* slider) override;
	
	private:
		const juce::AudioProcessorValueTreeState& apvts;
		const int maxNumTrees;
		int currentForestIndex;
		juce::StringArray treeContainer;

		juce::Slider* forestSlider = nullptr;
		const Preset::PresetManager& presetManager;
	};
}