#include "LSystemManager.h"
#include "Ids.h"
#include "PresetManager.h"
// Implement the next layer of stuff. 
// Now the actual shit that handles the growth logic for the l system shit.
LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, juce::Array<int>& _currenNotesPool)
	: apvts(_apvts), presetManager(_presetManager), currentNotesPool(_currenNotesPool)
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
	return;
	DBG("Configuring axiom input text editor");
	axiomInputEditor = textEditor;
	axiomInputEditor->setText(axiomInputValue.toString());
	axiomInputEditor->setInputRestrictions(1); // Restricts axiom to one character.
	axiomInputEditor->addListener(this);
}

void LSystemStuff::LSystemManager::configureRulesetInputTextEditor(juce::TextEditor* textEditor)
{
	return;
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
	gensKnob->addListener(this);
}

void LSystemStuff::LSystemManager::setCurrentNotesPool()
{
	DBG("Setting current notes pool");
	if (!currentNotesPool.isEmpty())
	{
		DBG("Current Notes Pool Size: " << currentNotesPool.size());
		currentNotesPool.clear();
	}
	juce::Array<juce::var> notesPoolTemp = *notesPoolValue.getValue().getArray(); // Should not be a nullptr, assuming generateNotesPool() succeeds.
	for (auto& item : notesPoolTemp)
	{
		currentNotesPool.add(static_cast<int>(item));
		DBG("Adding item to newNotesPool: " << item);
	}
}

void LSystemStuff::LSystemManager::buttonClicked(juce::Button* button)
{
	DBG("button click detected");
	DBG("Current Notes Pool Size: " << currentNotesPool.size());
	if (button == growButton)
	{
		lSystemProcessor->growLSystem();
		setCurrentNotesPool();
	}
}

void LSystemStuff::LSystemManager::textEditorTextChanged(juce::TextEditor& textEditor)
{
	DBG("Text editor(s) changed");
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

void LSystemStuff::LSystemManager::sliderValueChanged(juce::Slider* slider)
{
	if (slider == gensKnob)
	{
		DBG("Slider gens knob change detected!");
		presetManager.isModified = true;
	}
}

// This is called every time a preset is loaded (and/or we apvts.replaceState()).
void LSystemStuff::LSystemManager::valueTreeRedirected(juce::ValueTree& changedValueTree)
{
	DBG("Value tree redirected.");
	axiomInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomStringProperty, nullptr));
	rulesetInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr));
	generatedStringValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
	notesPoolValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));

	// When APVTS is refreshed, sets the values back to the appropriate ones. 
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