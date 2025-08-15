#include "LSystemManager.h"
#include "Ids.h"
#include "PresetManager.h"
#include "AxiomSelector.h"

// Implement the next layer of stuff. 
// Now the actual shit that handles the growth logic for the l system shit.

LSystemStuff::LSystemManager::LSystemManager(juce::AudioProcessorValueTreeState& _apvts, Preset::PresetManager& _presetManager, Tree::ActiveTreeManager* _activeTreeManager)
	: apvts(_apvts), presetManager(_presetManager), activeTreeManager(_activeTreeManager)
{
	lSystemProcessor = std::make_unique<LSystemProcessor>(gensKnob, _activeTreeManager, analogUserInputComponent);
	selectedAxiomId.setValue("");
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
		else if (child->getComponentID().startsWith(jiveGui::IdPrefix::inputBlock
		))
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

void LSystemStuff::LSystemManager::connectToJiveEditor(jive::GuiItem* _editor)
{
	jassert(mainEditor != _editor);

	mainEditor = _editor;
	lSystemProcessor->connectWithEditor(mainEditor);
}

void LSystemStuff::LSystemManager::configureAUIResetButton(juce::Component* component)
{
	juce::Button* button = dynamic_cast<juce::Button*>(component);
	resetButton = button;
	resetButton->addListener(this);
}

///TODO: DO WE EVEN NEED THIS SHIT??
// analoguserInputComponent is the topmost parent component for all the different input blocks.
void LSystemStuff::LSystemManager::configureAnalogUserInput(juce::Component* analogUserInputComponent, 
															const int numBlockRows, 
															const int numBlockColumns)
{
	DBG("Configuring Analog User Input");
	jassert(analogUserInputComponent != nullptr);
	this->analogUserInputComponent = analogUserInputComponent;
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

void LSystemStuff::LSystemManager::buttonClicked(juce::Button* button)
{
	DBG("button click detected in LSYSMANAGER");
	if (button == growButton)
	{
		lSystemProcessor->growLSystem();
	}
	else if (button = resetButton)
	{
		resetAUI();
	}
}

void LSystemStuff::LSystemManager::resetAUI()
{
	DBG("Resetting AUI");
	for (int row = 0; row < jiveGui::AnalogUserInput::NUMBLOCKROWS; row++)
	{
		for (int col = -1; col < jiveGui::AnalogUserInput::NUMBLOCKCOLUMNS; col++)
		{
			// NoteWheel
			const auto jiveNoteWheelId = jiveGui::idRowColMaker(jiveGui::IdPrefix::noteWheel, row, col);
			auto* noteWheel = dynamic_cast<juce::Slider*>(jive::findItemWithID(*mainEditor, jiveNoteWheelId)->getComponent().get());
			noteWheel->setValue(0);

			// Find Octaves
			const auto jiveOctavesId = jiveGui::idRowColMaker(jiveGui::IdPrefix::octavesInput, row, col);
			auto* octavesInput = dynamic_cast<juce::Slider*>(jive::findItemWithID(*mainEditor, jiveOctavesId)->getComponent().get());
			jassert(octavesInput != nullptr);
			octavesInput->setValue(0);

			// Find Direction
			const auto jiveDirectionId = jiveGui::idRowColMaker(jiveGui::IdPrefix::directionInput, row, col);
			auto* directionInput = dynamic_cast<juce::Button*>(jive::findItemWithID(*mainEditor, jiveDirectionId)->getComponent().get());
			jassert(directionInput != nullptr);
			directionInput->setToggleState(true, true);

			// Find if axiom
			///TODO: Currently axiom is not reset.
			if (col == -1)
			{
				const auto jiveAxiomId = jiveGui::idRowColMaker(jiveGui::IdPrefix::axiomToggle, row, col);
				auto* axiomInput = dynamic_cast<juce::Button*>(jive::findItemWithID(*mainEditor, jiveAxiomId)->getComponent().get());
				jassert(axiomInput != nullptr);
				
			}
		}
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