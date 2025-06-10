#include "LSystemManager.h"
#include "Ids.h"
#include "PresetManager.h"
#include "AnalogUserInputBlockData.h"

// Implement the next layer of stuff. 
// Now the actual shit that handles the growth logic for the l system shit.

LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, juce::Array<int>& _currenNotesPool)
	: apvts(_apvts), presetManager(_presetManager), currentNotesPool(_currenNotesPool)
{
	axiomInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userAxiomStringProperty, nullptr));
	rulesetInputValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::userRulesetStringProperty, nullptr));
	generatedStringValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::generatedLsysStringProperty, nullptr));
	notesPoolValue.referTo(apvts.state.getPropertyAsValue(apvtsPropIds::notesPoolVectorStringProperty, nullptr));

	lSystemProcessor = std::make_unique<LSystemProcessor>(gensKnob, rulesetInputValue, axiomInputValue, generatedStringValue, notesPoolValue, analogUserInputComponent);
	DBG("default ruleset is: " << rulesetInputValue.getValue());

	apvts.state.addListener(this);
}

void LSystemStuff::LSystemManager::configureInputBlockTop(juce::Component* inputBlockTopComponent)
{
	auto children = inputBlockTopComponent->getChildren();
	jassert(children.size() > 0); // Should have at least one child.
	for (auto child : children)
	{
		if (child->getComponentID().startsWith(jiveGui::IdPrefix::octavesInput))
		{
			// Configure Octave Input
			DBG("> Found OI");
			juce::Slider* octaveInput = dynamic_cast<juce::Slider*>(child);
			jassert(octaveInput != nullptr);
		}
		else if (child->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockAxiom))
		{
			// Configure Axiom Input
			juce::Button* axiomInputButton = dynamic_cast<juce::Button*>(child);
			jassert(axiomInputButton != nullptr);
		}
	}
}

void LSystemStuff::LSystemManager::configureInputBlockBot(juce::Component* inputBlockBottomComponent)
{
	juce::Button* directionButton = dynamic_cast<juce::Button*>(inputBlockBottomComponent);
	jassert(directionButton != nullptr); // Should have at least one child.
}

void LSystemStuff::LSystemManager::configureNoteWheel(juce::Component* noteWheelComponent)
{
	juce::Slider* noteWheel = dynamic_cast<juce::Slider*>(noteWheelComponent);
	jassert(noteWheel != nullptr);
}

// analoguserInputComponent is the topmost parent component for all the different input blocks.
void LSystemStuff::LSystemManager::configureAnalogUserInput(juce::Component* analogUserInputComponent, 
															const int numBlockRows, 
															const int numBlockColumns)
{
	DBG("Configuring Analog User Input");
	jassert(analogUserInputComponent != nullptr);
	this->analogUserInputComponent = analogUserInputComponent;
	for (auto* child : analogUserInputComponent->getChildren())
	{	
		// Access Rows
		if (child->getComponentID().startsWith(jiveGui::IdPrefix::inputRow))
		{
			DBG("Accessed row...");
			DBG("Row child num: " << child->getNumChildComponents());
			for (auto* rowChild : child->getChildren())
			{
				if (rowChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlock))
				{
					DBG("Accessing inputBlock...");
					DBG("inputBlock child num: " << rowChild->getNumChildComponents());
					AnalogUserInput::AnalogUserInputBlockData inputBlock;
					for (auto* blockChild : rowChild->getChildren())
					{
						DBG("INPUT BLOCKCHILD ID: " << blockChild->getComponentID());
						if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockTop))
						{
							// Found input block top.
							configureInputBlockTop(blockChild);
							DBG("Found an input block top!");
							DBG("ibt child num: " << blockChild->getNumChildComponents());
						}
						else if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::noteWheel))
						{
							// Found note wheel.
							DBG("Found a note wheel!");
							DBG("nw child num: " << blockChild->getNumChildComponents());
							configureNoteWheel(blockChild);
						}
						else if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockBottom))
						{
							// Found input block bottom.
							DBG("Found an input block bottom!");
							DBG("ibb child num: " << blockChild->getNumChildComponents());
							configureInputBlockBot(blockChild);
						}
					}
				}
			}
		}
	}
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
		// setCurrentNotesPool();
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