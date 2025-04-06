#include "LSystemManager.h"
#include "Ids.h"

//TODO: Figure out why teh value shit isn't working.
// problem is right here.
LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts)
	: apvts(_apvts)
{
	axiomInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomProperty, nullptr));
	rulesetInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetProperty, nullptr));
}

void LSystemStuff::LSystemManager::configureAxiomInputTextEditor(juce::TextEditor* textEditor)
{
	DBG("Configuring axiom input text editor");
	axiomInputEditor = textEditor;
	axiomInputEditor->setText(apvts.state.getProperty(apvtsPropIds::userAxiomProperty));
	axiomInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureRulesetInputTextEditor(juce::TextEditor* textEditor)
{
	DBG("Configuring ruleset input text editor");
	rulesetInputEditor = textEditor;
	rulesetInputEditor->setText(apvts.state.getProperty(apvtsPropIds::userRulesetProperty));
	rulesetInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureGrowButton(juce::Button* button)
{
	DBG("configured grow button");
	growButton = button;
	growButton->addListener(this);
}

void LSystemStuff::LSystemManager::buttonClicked(juce::Button* button)
{
	DBG("button click detected");
	if (button == growButton)
	{
		DBG("Plant button clicked");
		// Check if the axiom and ruleset inputs are valid
		// Generate dat shit.
		// lSystemProcessor.generateLSystem(apvts.getRawParameterValue("gens")->load());
	}
}

void LSystemStuff::LSystemManager::textEditorTextChanged(juce::TextEditor& textEditor)
{
	if (&textEditor == rulesetInputEditor)
	{
		DBG("inputEditor text changed.");
		//axiomInputValue = rulesetInputEditor->getText();
		//DBG("Change in text editor detected");
		//DBG("Current ruleSet: " << rulesetInputValue);
	}

	if (&textEditor == axiomInputEditor)
	{
		DBG("axiom text change");
		//rulesetInputValue = axiomInputEditor->getText();
		//DBG("Change in axiom editor detected");
		//DBG("Current axiom: " << axiomInputValue);
	}
}

void LSystemStuff::LSystemManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
{
	DBG("Value tree redirected.");
	// UPDATE ALL VALUES HERE.
}