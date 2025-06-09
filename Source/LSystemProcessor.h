// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"
#include "AnalogUserInputBlockData.h"
using namespace AnalogUserInput;

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

private:
    juce::Value& rulesetUserInput;          // juce::String
    juce::Value& axiomUserInput;            // juce::String
    juce::Value& generatedLString;          // juce::String
    juce::Value& notesPool;                 // juce::Array<juce::var>
	juce::Component*& analogUserInputComponent;

    juce::Slider*& generationsKnob; // Warning: this could be a nullptr at class's construction, so make sure to jassert that it isn't when we need it.
    int generationsNum;

    std::unordered_map<std::string, std::string> generateRulemap(std::vector<std::vector<AnalogUserInputBlockData>>&);
    std::vector<std::vector<AnalogUserInputBlockData>> makeAnalogUIBlockDataSet(); 
    void setBlockDataSetLSysChars(std::vector<std::vector<AnalogUserInputBlockData>>&);
    char getAxiomCharFromBDS(std::vector<std::vector<AnalogUserInputBlockData>>&) const;
    void generateLString(char&, std::unordered_map<std::string, std::string>&);
    void generateNotesPool(std::vector<std::vector<AnalogUserInputBlockData>>&);
};