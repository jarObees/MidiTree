// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"

//TODO: IMPLEMENT SAVING FEATURE!!!
// Should be able to save each lsystem into some sort of data format that can be shared across all instances
// of this plugin. Then you can browse and select a format that's already generated.
// Maybe even automate this. So you can combine L systems within the same thing.
class LSystemProcessor
{
public:
    LSystemProcessor(std::atomic<float>& generations);
    // L System Generation ====================================================
    juce::SortedSet<std::string> current_lsysVars;
    juce::SortedSet<std::string> current_lsysRulesets;
    std::string lsysAxiom;
    std::vector<LSystem> lSystems;
    void generateLSystem(const uint8_t& gens);
    // APVTS Processor ========================================================
    std::vector<int> generateNotesPool(const std::string& genString);
    // l-string generated notes relative to the axiom. 
    // Currently used for storing the actual note pool.
    //TODO: Fix how note pools are stored. They should be stored as a saved parameter or something. 
    std::vector<int> notesPool;
    static inline const std::unordered_map<std::string, float> noteRateMap = {
        {"32/1", 32.0f}, {"16/1", 16.0f},
        {"8/1", 8.0f}, {"4/1", 4.0f},
        {"2/1", 2.0f}, {"1/1", 1.0f},
        {"1/2", 0.5f}, {"1/4", 0.25f},
        {"1/8", 0.125f}, {"1/16", 0.0625f},
        {"1/32", 0.03125f}, {"1/64", 0.015625f}
    };
    std::vector<std::string> noteRateKeys;
private:
    std::atomic<float>& generations;
    // Used for replacing user input (keys), to single char strings to be used in l-sys computation (values).
    static inline const std::unordered_map<std::string, std::string> replacementRulesToChar = {
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
    std::unordered_map<std::string, std::string> generateRuleset();
    void saveLSystem(LSystem lSystem);
};