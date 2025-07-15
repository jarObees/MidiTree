#pragma once
#include <JuceHeader.h>
#include "Tree.h"
namespace Preset { class PresetManager; }
namespace InfoTabManager { class InfoTabManager; }

namespace Forest
{
	namespace midiTreePair
	{
		static const juce::String lStringToShow = "lstringtoshow";
		static const juce::String lStringToComputer = "lstringtocomputer";
	}

	class ForestManager
		: public juce::Slider::Listener, juce::Button::Listener, juce::ValueTree::Listener, juce::MouseListener
	{
	public:
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&, juce::Array<int>& currentNotesPool, InfoTabManager::InfoTabManager*);
		void configureTreeSlider(juce::Slider* slider);
		void configureForestButton(juce::Button* button);
		void configureForestTrees(std::vector<jiveGui::ForestUI::TreeComponent*>);
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		void valueTreeRedirected(juce::ValueTree& changedTree) override;
		void mouseEnter(const juce::MouseEvent& event) override;
		void mouseExit(const juce::MouseEvent& event) override;
		const int maxNumTrees;

	private:
		InfoTabManager::InfoTabManager* infoTabManager;
		bool bypass;	// Tells plugin to bypass the forest, and just play whatever thing has been immediately loaded.
		juce::Value generatedLString; // juce::String
		juce::Value notesPool; // juce::Array<juce::var>
		juce::Value midiTreeName; // juce::Array<juce::var>
		juce::Array<int>& currentNotesPool;

		juce::AudioProcessorValueTreeState& apvts;
		int currentForestIndex;

		// Slider will be indexing across this vector. 
		std::vector<std::pair<juce::String, juce::Array<int>>> forestDataSlots; // First pair value is the midiTree name, second is the notesPool.
		std::vector<jiveGui::ForestUI::TreeComponent*> forestTrees;
		juce::Slider* forestSlider = nullptr;
		juce::Button* forestButton = nullptr;

		Preset::PresetManager& presetManager;

		void plantTree();
	};
}