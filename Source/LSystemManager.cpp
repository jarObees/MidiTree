#include "LSystemManager.h"
#include "Ids.h"

// Implement the next layer of stuff. 
// Now the actual shit that handles the growth logic for the l system shit.
LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts)
	: apvts(_apvts)
{
	axiomInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomStringProperty, nullptr));
	rulesetInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr));
	generatedStringValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
	notesPoolValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));

	lSystemProcessor = std::make_unique<LSystemProcessor>(gensKnob, rulesetInputValue, axiomInputValue, generatedStringValue, notesPoolValue);
}

void LSystemStuff::LSystemManager::configureAxiomInputTextEditor(juce::TextEditor* textEditor)
{
	DBG("Configuring axiom input text editor");
	axiomInputEditor = textEditor;
	axiomInputEditor->setText(axiomInputValue.toString());
	axiomInputEditor->setInputRestrictions(1); // Restricts axiom to one character.
	axiomInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureRulesetInputTextEditor(juce::TextEditor* textEditor)
{
	DBG("Configuring ruleset input text editor");
	rulesetInputEditor = textEditor;
	rulesetInputEditor->setText(rulesetInputValue.toString());
	rulesetInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureGrowButton(juce::Button* button)
{
	DBG("configured grow button");
	growButton = button;
	growButton->addListener(this);
}

void LSystemStuff::LSystemManager::configureGensKnob(juce::Slider* knob)
{
	DBG("configured gens knob");
	gensKnob = knob;
	jassert(gensKnob != nullptr);
}

void LSystemStuff::LSystemManager::buttonClicked(juce::Button* button)
{
	DBG("button click detected");
	if (button == growButton)
	{
		DBG("Grow button clicked.");
		lSystemProcessor->growLSystem();
	}
}

void LSystemStuff::LSystemManager::textEditorTextChanged(juce::TextEditor& textEditor)
{
	if (&textEditor == rulesetInputEditor)
	{
		DBG("inputEditor text changed.");
		rulesetInputValue = rulesetInputEditor->getText();
		//DBG("Change in text editor detected");
		//DBG("Current ruleSet: " << rulesetInputValue);
	}

	if (&textEditor == axiomInputEditor)
	{
		DBG("axiom text change");
		axiomInputValue = axiomInputEditor->getText();
		//DBG("Change in axiom editor detected");
		//DBG("Current axiom: " << axiomInputValue);
	}
}

void LSystemStuff::LSystemManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
{
	DBG("Value tree redirected.");
	// UPDATE ALL VALUES HERE.
}