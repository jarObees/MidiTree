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
		: public juce::Slider::Listener, juce::Button::Listener, juce::ValueTree::Listener
	{
	public:
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&);
		void configureTreeSlider(juce::Slider* slider);
		void configureForestButton(juce::Button* button);
		
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		void valueTreeRedirected(juce::ValueTree& changedTree) override;
	
	private:
		juce::Value generatedLString;
		juce::Value notesPool; // juce::Array<juce::var>

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