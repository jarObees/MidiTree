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
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&, juce::Array<int>& currentNotesPool);
		void configureTreeSlider(juce::Slider* slider);
		void configureForestButton(juce::Button* button);
		
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		void valueTreeRedirected(juce::ValueTree& changedTree) override;
	
	private:
		juce::Value generatedLString; // juce::String
		juce::Value notesPool; // juce::Array<juce::var>
		juce::Value midiTreeName; // juce::Array<juce::var>
		juce::Array<int>& currentNotesPool;

		juce::AudioProcessorValueTreeState& apvts;
		const int maxNumTrees;
		int currentForestIndex;

		// Slider will be indexing across this vector.
		// First pair value is the midiTree name, second is the notesPool.
		std::vector<std::pair<juce::String, juce::Array<int>>> forestDataSlots;
		
		juce::Slider* forestSlider = nullptr;
		juce::Button* forestButton = nullptr;

		const Preset::PresetManager& presetManager;

		void plantTree();
	};
}