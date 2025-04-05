#pragma once
#include <JuceHeader.h>
#include "LSystemProcessor.h"
namespace LSystemStuff
{
	class LSystemManager
		: public juce::TextEditor::Listener, juce::Button::Listener
	{
	public:
		LSystemManager(juce::AudioProcessorValueTreeState& _apvts);
		void configureAxiomInputTextEditor(juce::TextEditor* textEditor);
		void configureRulesetInputTextEditor(juce::TextEditor* textEditor);
		void configureGrowButton(juce::Button* button);

	private:
		void textEditorTextChanged(juce::TextEditor& textEditor) override;
		void buttonClicked(juce::Button* button) override;

		//LSystemProcessor lSystemProcessor;
		juce::TextEditor* axiomInputEditor = nullptr;
		juce::TextEditor* rulesetInputEditor = nullptr;
		juce::Button* growButton;

		juce::String axiomInputString;
		juce::String rulesetInputString;
		juce::AudioProcessorValueTreeState& apvts;
	};
}