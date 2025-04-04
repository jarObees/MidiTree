#pragma once
#include <JuceHeader.h>

namespace Preset { class PresetManager; }

namespace Forest
{
	class ForestManager
		: public juce::Slider::Listener
	{
	public:
		ForestManager(Preset::PresetManager&, const int _numOTrees);
		void plantTree();
		void configureTreeSlider(juce::Slider* slider);
		void sliderValueChanged(juce::Slider* slider) override;
	
	private:
		const int maxNumTrees;
		int currentForestIndex;
		juce::StringArray treeContainer;

		juce::Slider* forestSlider = nullptr;
		const Preset::PresetManager& presetManager;

	};
}