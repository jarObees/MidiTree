#pragma once
#include <JuceHeader.h>

//TODO: One day refactor these to just be under the same namespace. Not important for now lol.
namespace jive_gui
{
	// All ID's for a component should be stored here.
	namespace StringIds
	{
		const juce::String presetComboBox{ "preset-comboBox" };	
		const juce::String forestSlider{ "forest-slider" };
		const juce::String saveButton{ "save-button" };
		const juce::String rulesetTextbox{ "ruleset-textBox" };
		const juce::String plantButton{ "load-button" };
		const juce::String generationsKnob{ "generations-knob" };
		const juce::String growButton{ "grow-button" };
		const juce::String midiVelocityKnob{ "midiVelocity-knob" };
		const juce::String noteRateKnob{ "noteRate-knob" };
		const juce::String noteTypeComboBox{ "noteType-comboBox" };
		const juce::String nameTextBox{ "name-textBox" };
		const juce::String axiomTextBox{ "axiom-textBox" };
	}
}

// All ID's for properties should be stored here.
namespace apvtsPropIds
{
	static const juce::Identifier userRulesetProperty{ "userRulesetNode" };
	static const juce::Identifier userAxiomProperty{ "userAxiomNode" };
	static const juce::Identifier userLsysNameProperty{ "userLsysNameNode" };
	static const juce::Identifier generatedLsysStringProperty{ "generatedLStringNode" };
	static const juce::Identifier notesPoolProperty{ "notesPoolNode" };
	static const juce::Identifier presetNameProperty{ "presetNameProperty" };
}