// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

// This class is where the actual calculations for the L-System are made.
class LSystemProcessor
{
public:
    LSystemProcessor(juce::Slider*& generationsKnob, 
                     juce::Value& _rulesetUserInput, 
                     juce::Value& _axiomUserInput, 
                     juce::Value& _generatedLString,
                     juce::Value& _notesPool,
                     juce::Component*& analogUserInputComponent);
    void growLSystem();


    // APVTS Processor ========================================================
    std::vector<std::string> noteRateKeys;
private:
    juce::Value& rulesetUserInput;          // juce::String
    juce::Value& axiomUserInput;            // juce::String
    juce::Value& generatedLString;          // juce::String
    juce::Value& notesPool;                 // juce::Array<juce::var>
	juce::Component*& analogUserInputComponent;

    juce::Slider*& generationsKnob; // Warning: this could be a nullptr at class's construction, so make sure to jassert that it isn't when we need it.
    int generationsNum;
    bool setLSystemRulesAndVariables();
    bool confirmAxiom();
	juce::SortedSet<std::string> currentLSystemVariables; // SortedSet of variables, each variable is a string like "1", "b2", "#3", etc...
	juce::SortedSet<std::string> currentLSystemRules; // SortedSet of rules, each rule is a string like "1=#2b4", "b4=2b34#4", etc...
    std::unordered_map<std::string, std::string> generateRulemap();
	std::unordered_map<std::string, std::string> currentLSystemRulemap; // Maps each variable to its corresponding rule, e.g., {"1": "#2b4", "b4": "2b34#4"}. Used directly in L-system generation.
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
        {"#1", "A"}, {"b2", "A"},
        {"#2", "B"}, {"b3", "B"},
        {"#4", "C"}, {"b5", "C"},
        {"#5", "D"}, {"b6", "D"},
        {"#6", "E"}, {"b7", "E"},
        {"b4", "3"}, {"#3", "4"}
    };

    // TODO: Realizing a big issue. What about notes that are octaves.
    // Might need to rethink this hol unordered map shit.
    // Or at least. Let's think. if a character goes beyond an octave. We have to adjust our rule maps.
    // Because ultimately, each interval does indeed require a unique symbol.
    static inline const std::unordered_map<std::string, int> replacementRulesToInt = {
        {"1", 0}, {"A", 1},
        {"2", 2}, {"B", 3},
        {"3", 4}, {"C", 6},
        {"4", 5}, {"D", 8},
        {"5", 7}, {"E", 10},
        {"6", 9}, {"7", 11}
    };
};