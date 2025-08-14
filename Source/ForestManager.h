#pragma once
#include <JuceHeader.h>
#include "Tree.h"
#include "TreeData.h"
#include "ActiveTreeManager.h"

// FINISH IMPLEMENTING ACTIVETREE
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
		ForestManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager&, Tree::ActiveTreeManager*, InfoTabManager::InfoTabManager*);
		void configureTreeSlider(juce::Slider* slider);
		void configurePlantButtonForest(juce::Button* button);
		void configureForestTrees(std::vector<jiveGui::ForestUI::TreeComponent*>);
		void configureForestBypassButton(juce::Button*);
		void configureForestResetButton(juce::Button*);

		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		void valueTreeRedirected(juce::ValueTree& changedTree) override;
		void mouseEnter(const juce::MouseEvent& event) override;
		void mouseExit(const juce::MouseEvent& event) override;
		const int maxNumTrees;
		bool isBypass() const;
		void setBypass(bool);

	private:
		InfoTabManager::InfoTabManager* infoTabManager;
		Tree::ActiveTreeManager* activeTreeManager;
		bool bypassState;	// Tells plugin to bypass the forest, and just play whatever thing has been immediately loaded. TODO: Should this be automatable?
		juce::Value generatedLString; // juce::String
		juce::Value notesPool; // juce::Array<juce::var>
		juce::Value midiTreeName; // juce::Array<juce::var>

		juce::AudioProcessorValueTreeState& apvts;
		int currentForestIndex;

		// Slider will be indexing across this vector. 
		std::vector<Tree::MidiTree> forestDataSlots;
		std::vector<jiveGui::ForestUI::TreeComponent*> forestTreesUI;
		juce::Slider* forestSlider = nullptr;
		juce::Button* plantButton = nullptr;
		juce::Button* bypassButton = nullptr;
		juce::Button* resetButton = nullptr;

		Preset::PresetManager& presetManager;

		void plantTree();
		void resetForestSlots();
		void updateTreeSlotsUI();
	};
}