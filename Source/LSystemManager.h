#pragma once
#include <JuceHeader.h>
#include "LSystemProcessor.h"
#include "PresetManager.h"
#include "TreeData.h"

namespace LSystemStuff
{
	// Manages the GUI/Non-auto params to be used in L-System generation.
	// Do we even need this anymore?????
	class LSystemManager
		: public juce::Button::Listener, juce::Slider::Listener
	{
	public:
		LSystemManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, Tree::MidiTree* _activeTree);
		void configureGensKnob(juce::Slider* gensKnob);
		void configureGrowButton(juce::Button* button);
		void configureAnalogUserInput(juce::Component* analogUserInputComponent, const int numBlockRows, const int numBlockColumns);
		void configureInputBlockTop(juce::Component* octaveBlockTopComponent);
		void configureInputBlockBot(juce::Component* inputBlockBottomComponent);
		void configureNoteWheel(juce::Component* noteWheelComponent);
		void connectAxiomButton(juce::Component* axiomButton);
		juce::Value selectedAxiomId;

	private:
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		
		Tree::MidiTree* activeTree;
		juce::TextEditor* axiomInputEditor = nullptr;
		juce::TextEditor* rulesetInputEditor = nullptr;
		juce::Button* growButton = nullptr;
		juce::Slider* gensKnob = nullptr;
		juce::Component* analogUserInputComponent = nullptr;
		std::vector<juce::Button*> axiomSelectorButtonGroup;
		juce::AudioProcessorValueTreeState& apvts;
		Preset::PresetManager& presetManager;
		std::unique_ptr<LSystemProcessor> lSystemProcessor;
	};
}