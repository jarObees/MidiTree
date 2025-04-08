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
	DBG("default ruleset is: " << rulesetInputValue.getValue());

	apvts.state.addListener(this);
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
	rulesetInputEditor->setText("DEFAULT TEXT");
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
	gensKnob->setValue(3);
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
	DBG("ruleset before making changes to text editor: " << rulesetInputValue);
	if (&textEditor == rulesetInputEditor)
	{
		rulesetInputValue = rulesetInputEditor->getText();

		//DBG("Change in text editor detected");
		//DBG("Current ruleSet: " << rulesetInputValue);
	}

	if (&textEditor == axiomInputEditor)
	{
		axiomInputValue = axiomInputEditor->getText();
		//DBG("Change in axiom editor detected");
		//DBG("Current axiom: " << axiomInputValue);
	}
}

// This is called every time a preset is loaded (or we apvts.replaceState()).
void LSystemStuff::LSystemManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
{
	DBG("Value tree redirected.");
	axiomInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomStringProperty, nullptr));
	rulesetInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr));
	generatedStringValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
	notesPoolValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));
	if (axiomInputEditor != nullptr && rulesetInputEditor != nullptr)
	{
		axiomInputEditor->setText(axiomInputValue.toString());
		rulesetInputEditor->setText(rulesetInputValue.toString());
	}
}

void LSystemStuff::LSystemManager::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property)
{
	if (property == apvtsPropIds::userRulesetStringProperty)
	{
		DBG("!" << apvtsPropIds::userRulesetStringProperty << "property changed to: " << tree.getProperty(property).toString());
	}
	if (property == apvtsPropIds::generatedLsysStringProperty)
	{
		DBG("!" << apvtsPropIds::generatedLsysStringProperty << "property changed to: " << tree.getProperty(property).toString());
	}
}