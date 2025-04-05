#include "LSystemManager.h"
#include "Ids.h"

LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts)
	: apvts(_apvts) {}

void LSystemStuff::LSystemManager::configureAxiomInputTextEditor(juce::TextEditor* textEditor)
{
	axiomInputEditor = textEditor;
	axiomInputEditor->setText(apvts.state.getProperty(apvtsPropIds::userAxiomProperty));
	axiomInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureRulesetInputTextEditor(juce::TextEditor* textEditor)
{
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
		DBG("Change in text editor detected");
		apvts.state.setProperty(apvtsPropIds::userRulesetProperty,
								rulesetInputEditor->getText(),
								nullptr);
		// rulesetInputString = apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetProperty, nullptr);
	}

	if (&textEditor == axiomInputEditor)
	{
		DBG("Change in axiom editor detected");
		apvts.state.setProperty(apvtsPropIds::userAxiomProperty, 
								axiomInputEditor->getText(), 
								nullptr);
		// axiomInputString = apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomProperty, nullptr);
	}
}