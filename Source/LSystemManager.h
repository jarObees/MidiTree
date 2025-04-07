#pragma once
#include <JuceHeader.h>
#include "LSystemProcessor.h"

namespace LSystemStuff
{
	class LSystemManager
		: public juce::TextEditor::Listener, juce::Button::Listener, juce::ValueTree::Listener
	{
	public:
		LSystemManager(juce::AudioProcessorValueTreeState& _apvts);
		void configureAxiomInputTextEditor(juce::TextEditor* textEditor);
		void configureRulesetInputTextEditor(juce::TextEditor* textEditor);
		void configureGensKnob(juce::Slider* gensKnob);
		void configureGrowButton(juce::Button* button);
		void valueTreeRedirected(juce::ValueTree& changedValueTree) override;

	private:
		void textEditorTextChanged(juce::TextEditor& textEditor) override;
		void buttonClicked(juce::Button* button) override;

		//LSystemProcessor lSystemProcessor;
		juce::Value axiomInputValue;
		juce::Value rulesetInputValue;
		juce::Value generatedStringValue;
		juce::Value notesPoolValue;

		juce::TextEditor* axiomInputEditor = nullptr;
		juce::TextEditor* rulesetInputEditor = nullptr;
		juce::Button* growButton = nullptr;
		juce::Slider* gensKnob = nullptr;

		juce::AudioProcessorValueTreeState& apvts;
		std::unique_ptr<LSystemProcessor> lSystemProcessor;
	};
}