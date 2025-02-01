#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

class LSystemProcessor
{
public:
	juce::SortedSet<std::string> current_lsysVars;
	juce::SortedSet<std::string> current_lsysRulesets;
	std::string current_axiom;
	std::vector<LSystem> lSystems;

	void generateLSystem(const uint8_t& gens)
	{
		DBG("Begin Generating L System------");
		DBG("Translating rulesets/vars");
		translateSet(current_lsysRulesets);
		translateSet(current_lsysVars);

		std::unordered_map<std::string, std::string> newLsysRulemap = generateRuleset();
		LSystem lsystem(current_axiom, newLsysRulemap);
		lSystems.push_back(lsystem);
		lsystem.generate(generations);
	}

	LSystemProcessor(std::atomic<float>& generations)
		:generations(generations) {}
private:
	std::atomic<float>& generations;
	// Map containing illegal strings as keys, and their corrosponding legal string as values.
	// For use in correcting lsys variables and rulesets.
	std::unordered_map<std::string, std::string> REPLACEMENTRULES =
	{
		{"#1", "a"}, {"b2", "a"},
		{"#2", "b"}, {"b3", "b"},
		{"#4", "c"}, {"b5", "c"},
		{"#5", "d"}, {"b6", "d"},
		{"#6", "e"}, {"b7", "e"},
		{"b4", "3"}, {"#3", "4"}
	};

	// We need to verify that all variables in l system are single characters for future rule application.
	// Searches through each string in the set and replaces illegal substrings with legal ones using the replacementRules map.
	void translateSet(juce::SortedSet<std::string>& stringSet)
	{
		juce::SortedSet<std::string> translatedRuleset;
		// Iterates through each line in the ruleset.
		for (const std::string& ruleLine : stringSet)
		{
			std::string newRuleLine = ruleLine;
			// Iterates through each key, value pair in map.
			for (const auto& [key, value] : REPLACEMENTRULES)
			{
				// Replace any instance of the key (illegal string) in the ruleLine with the value (legal string)
				size_t pos = 0;
				while ((pos = newRuleLine.find(key, pos)) != std::string::npos)
				{
					newRuleLine.replace(pos, key.length(), value);
					pos += value.length();
				}
			}
			translatedRuleset.add(newRuleLine);
		}
		stringSet = std::move(translatedRuleset);
	}

	// Generates ruleset map from ruleset set.
	std::unordered_map<std::string, std::string> generateRuleset()
	{
		DBG("Generating Ruleset");
		std::unordered_map<std::string, std::string> map;
		for (const std::string& ruleLine : current_lsysRulesets)
		{
			size_t equalsPos = ruleLine.find('=');
			std::string leftHandSide = ruleLine.substr(0, equalsPos);
			std::string rightHandSide = ruleLine.substr(equalsPos + 1);
			map.emplace(leftHandSide, rightHandSide);
		}
		return map;
	}
};