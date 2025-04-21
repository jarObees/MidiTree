// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

class LSystemProcessor
{
public:
    LSystemProcessor(juce::Slider*& generationsKnob, 
                     juce::Value& _rulesetUserInput, 
                     juce::Value& _axiomUserInput, 
                     juce::Value& _generatedLString,
                     juce::Value& _notesPool);
    void growLSystem();


    // APVTS Processor ========================================================
    std::vector<std::string> noteRateKeys;
private:
    juce::Value& rulesetUserInput;          // juce::String
    juce::Value& axiomUserInput;            // juce::String
    juce::Value& generatedLString;          // juce::String
    juce::Value& notesPool;                 // juce::Array<juce::var>

    juce::Slider*& generationsKnob; // Warning: this could be a nullptr at class's construction, so make sure to jassert that it isn't when we need it.
    int generationsNum;
    bool setLSystemRulesAndVariables();
    bool confirmAxiom();
    juce::SortedSet<std::string> currentLSystemVariables;
    juce::SortedSet<std::string> currentLSystemRules;
    std::unordered_map<std::string, std::string> generateRuleset();
	std::unordered_map<std::string, std::string> currentLSystemRulemap;
    void generateLString();
    void generateNotesPool();

    std::regex rulesetPattern{"^[b#]?[1-7]=([b#]?[1-7])+$"}; // Used to capture strings like, "1=#2b4", "b4=2b34#4", etc...
    std::regex variablePattern{"[b#]?[1-7]"}; // Individually captures strings like, "1", "#2", "b7"   
    std::string lsysAxiom;
    std::vector<LSystem> lSystems;
    void generateLSystem(const uint8_t& gens);
    void translateSet(juce::SortedSet<std::string>& stringSet);

    // Used for replacing user input (keys), to single char strings to be used in l-sys computation (values).
    static inline const std::unordered_map<std::string, std::string> replacementRulesToChar = 
    {
        {"#1", "a"}, {"b2", "a"},
        {"#2", "b"}, {"b3", "b"},
        {"#4", "c"}, {"b5", "c"},
        {"#5", "d"}, {"b6", "d"},
        {"#6", "e"}, {"b7", "e"},
        {"b4", "3"}, {"#3", "4"}
    };

    // TODO: Realizing a big issue. What about notes that are octaves.
    // Might need to rethink this hol unordered map shit.
    // Or at least. Let's think. if a character goes beyond an octave. We have to adjust our rule maps.
    // Because ultimately, each interval does indeed require a unique symbol.
    static inline const std::unordered_map<std::string, int> replacementRulesToInt = {
        {"1", 0}, {"a", 1},
        {"2", 2}, {"b", 3},
        {"3", 4}, {"c", 6},
        {"4", 5}, {"d", 8},
        {"5", 7}, {"e", 10},
        {"6", 9}, {"7", 11}
    };
};