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
	axiomInputEditor = textEditor;
	axiomInputEditor->setText(apvts.state.getProperty(apvtsPropIds::userAxiomProperty));
	axiomInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureRulesetInputTextEditor(juce::TextEditor* textEditor)
{
	DBG("Configuring userRulesetProperty");
	rulesetInputEditor = textEditor;
	rulesetInputEditor->setText(apvts.state.getProperty(apvtsPropIds::userRulesetProperty));
	rulesetInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureGrowButton(juce::Button* button)
{
	growButton = button;
	growButton->addListener(this);
}

void LSystemStuff::LSystemManager::buttonClicked(juce::Button* button)
{
	if (button == growButton)
	{
		// Check if the axiom and ruleset inputs are valid
		// Generate dat shit.
		// lSystemProcessor.generateLSystem(apvts.getRawParameterValue("gens")->load());
	}
}

void LSystemStuff::LSystemManager::textEditorTextChanged(juce::TextEditor& textEditor)
{
	if (&textEditor == rulesetInputEditor)
	{
		//axiomInputValue = rulesetInputEditor->getText();
		//DBG("Change in text editor detected");
		//DBG("Current ruleSet: " << rulesetInputValue);
	}

	if (&textEditor == axiomInputEditor)
	{
		//rulesetInputValue = axiomInputEditor->getText();
		//DBG("Change in axiom editor detected");
		//DBG("Current axiom: " << axiomInputValue);
	}
}

void LSystemStuff::LSystemManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
{
	// UPDATE ALL VALUES HERE.
}