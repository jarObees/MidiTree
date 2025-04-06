// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

class LSystemProcessor
{
public:
    LSystemProcessor(juce::Slider* generationsKnob, juce::Value& _rulesetUserInput, juce::Value& _axiomUserInput, juce::Value& _generatedLString);
    void growLSystem();


    // APVTS Processor ========================================================
    std::vector<int> notesPool; // Currently used for storing the actual note pool.
    // This shit has NO business being defined in the LSystemProcessor. It's only used in the audio processor stuff, so go define it over there.
    static inline const std::unordered_map<std::string, float> noteRateMap = {
        {"32/1", 32.0f}, {"16/1", 16.0f},
        {"8/1", 8.0f}, {"4/1", 4.0f},
        {"2/1", 2.0f}, {"1/1", 1.0f},
        {"1/2", 0.5f}, {"1/4", 0.25f},
        {"1/8", 0.125f}, {"1/16", 0.0625f},
        {"1/32", 0.03125f}, {"1/64", 0.015625f}
    };
    std::vector<std::string> noteRateKeys;
    void saveLSystem(std::unordered_map<std::string, juce::ValueTree*>& nonAutomatableParams);
private:
    juce::Value rulesetUserInput;
    juce::Value axiomUserInput;
    juce::Value generatedLString;

    juce::Slider* generationsKnob; // Warning: this could be a nullptr at class's construction, so make sure to jassert that it isn't when we need it.
    int generationsNum;
    bool setLSystemRules();
    bool confirmAxiom();
    juce::SortedSet<std::string> currentLSystemVariables;
    juce::SortedSet<std::string> currentLSystemRules;
    std::unordered_map<std::string, std::string> generateRuleset();
	std::unordered_map<std::string, std::string> currentLSystemRulemap;
    void generateLSystem();

    // L System Generation ====================================================
    std::string lsysAxiom;
    std::vector<LSystem> lSystems;
    void generateLSystem(const uint8_t& gens);
    std::vector<int> generateNotesPool(const std::string& genString);
    
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
    void translateSet(juce::SortedSet<std::string>& stringSet);
};