#pragma once
#include <JuceHeader.h>
#include "LSystemProcessor.h"
#include "PresetManager.h"

// 
namespace LSystemStuff
{
	// Manages the GUI/Non-auto params to be used in L-System generation.
	class LSystemManager
		: public juce::TextEditor::Listener, juce::Button::Listener, juce::ValueTree::Listener, juce::Slider::Listener
	{
	public:
		LSystemManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, juce::Array<int>& curentNotesPool);
		void configureAxiomInputTextEditor(juce::TextEditor* textEditor);
		void configureRulesetInputTextEditor(juce::TextEditor* textEditor);
		void configureGensKnob(juce::Slider* gensKnob);
		void configureGrowButton(juce::Button* button);
		void configureAnalogUserInput(juce::Component* analogUserInputComponent, const int numBlockRows, const int numBlockColumns);
		void configureInputBlockTop(juce::Component* octaveBlockTopComponent);
		// void configureNoteWheel(juce::Slider* noteWheel);
		// void configureInputBlockBottom(juce::Component* inputBlockBottomComponent);

	private:
		void valueTreeRedirected(juce::ValueTree& changedValueTree) override;
		void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override;
		void textEditorTextChanged(juce::TextEditor& textEditor) override;
		void buttonClicked(juce::Button* button) override;
		void sliderValueChanged(juce::Slider* slider) override;
		void setCurrentNotesPool();
			
		// Values are used to keep non-auto params connceted to apvts as var properties of the apvts.
		juce::Value axiomInputValue; //juce::String
		juce::Value rulesetInputValue; //juce::String
		juce::Value generatedStringValue; //juce::String
		juce::Value notesPoolValue; // juce::Array<juce::var>

		juce::TextEditor* axiomInputEditor = nullptr;
		juce::TextEditor* rulesetInputEditor = nullptr;
		juce::Button* growButton = nullptr;
		juce::Slider* gensKnob = nullptr;

		juce::AudioProcessorValueTreeState& apvts;
		Preset::PresetManager& presetManager;
		juce::Array<int> &currentNotesPool;
		std::unique_ptr<LSystemProcessor> lSystemProcessor;
	};
}