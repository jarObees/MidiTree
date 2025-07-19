#pragma once
#include <JuceHeader.h>

//TODO: One day refactor these to just be under the same namespace. Not important for now lol.
namespace jiveGui
{
	// Creates an ID with the form "prefixXY" where X and Y are the row and column respectively.
	[[nodiscard]] inline const juce::String idRowColMaker(const juce::String prefix, const int rowNum, const int columnNum)
	{
		DBG("Making ID with prefix: " << prefix << " rowNum: " << rowNum << " columnNum: " << columnNum);
		juce::String id = prefix + static_cast<juce::String>(rowNum) + static_cast<juce::String>(columnNum);
		DBG("Id Made! : " << id);
		return id;
	}
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
		const juce::String midiVelocityKnobText{ "mkt"};
		const juce::String noteRateKnob{ "noteRate-knob" };
		const juce::String noteRateKnobText{ "nrkt" };
		const juce::String noteTypeComboBox{ "noteType-comboBox" };
		const juce::String nameTextBox{ "name-textBox" };
		const juce::String axiomTextBox{ "axiom-textBox" };
		const juce::String analogUserInput{ "aui" };
		const juce::String forestTree{ "tre" };
		const juce::String forestBypassButton{ "fbb" };
		const juce::String activeAxiom{ "aa" };
	}
	namespace IdPrefix // Should only be two characters
	{
		const juce::String inputBlock{ "ib" };
		const juce::String inputBlockAxiom{ "ia" };
		const juce::String noteWheel{ "nw" };
		const juce::String inputBlockTop{ "ibt" };
		const juce::String inputBlockBottom{ "ibb" };
		const juce::String octavesInput{ "oi" };
		const juce::String directionInput{ "di" };
		const juce::String inputRow{ "ir" };
		const juce::String axiomToggle{ "ai" };
	}
	namespace ImageIds
	{
		const std::string mainKnobFilmstrip{ "mainKnobFilmstrip" };
		const std::string horiKnobFilmstrip{ "horiKnobFilmstrip" };
		const std::string octavesBackground1{ "octback1" };
	}

	namespace ForestUI
	{
		// Makes a unique id: treeIDPrefix + [tree number]
		[[nodisacrd]] inline const juce::String treeIdMaker(const int treeNum)
		{
			DBG("Making tree string: " << StringIds::forestTree + static_cast<juce::String>(treeNum));
			return StringIds::forestTree + static_cast<juce::String>(treeNum);
		}
	}

	namespace InfoTabUI
	{
		const juce::String textComponentID{ "itt" };
	}
}

// All ID's for properties should be stored here.
namespace apvtsPropIds
{
	static const juce::Identifier userRulesetStringProperty{ "userRulesetNode" };
	static const juce::Identifier userAxiomStringProperty{ "userAxiomNode" };
	static const juce::Identifier userLsysNameStringProperty{ "userLsysNameNode" };
	static const juce::Identifier generatedLsysStringProperty{ "generatedLStringNode" };
	static const juce::Identifier notesPoolVectorStringProperty{ "notesPoolNode" };
	static const juce::Identifier presetNameProperty{ "presetNameProperty" };
}

namespace dummyApvtsParamPrefix
{
	const juce::String noteWheelParam{ "dnw" };
	const juce::String octavesParam{ "doi" };
	const juce::String directionParam{ "ddi" };
	const juce::String axiomToggleParam{ "dai" };
}

namespace PluginTag
{
	const juce::String pluginTag{ "mapcat" };
}