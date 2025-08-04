// LSystemProcessor.h
#pragma once

#include "JuceHeader.h"
#include "LSystem.h"
#include "AnalogUserInputBlockData.h"
#include "TreeData.h"
#include "ActiveTreeManager.h"

using namespace AnalogUserInput;

// This class is where the actual calculations for the L-System are made.
class LSystemProcessor
{
public:
    LSystemProcessor(juce::Slider*& _generationsKnob,
                     Tree::ActiveTreeManager* _activeTree,
                     juce::Component*& _analogUserInputComponent);
    
    void growLSystem();

private:
	juce::Component*& analogUserInputComponent;
    Tree::ActiveTreeManager* activeTreeManager;
    juce::Slider*& generationsKnob; // Warning: this could be a nullptr at class's construction, so make sure to jassert that it isn't when we need it.
    int generationsNum;

    juce::String generatedLString;
    juce::Array<int> generatedNotesPool;

    std::unordered_map<std::string, std::string> generateRulemap(std::vector<std::vector<AnalogUserInputBlockData>>&);
    std::vector<std::vector<AnalogUserInputBlockData>> makeAnalogUIBlockDataSet(); 
    void setBlockDataSetLSysChars(std::vector<std::vector<AnalogUserInputBlockData>>&);
    char getAxiomCharFromBDS(std::vector<std::vector<AnalogUserInputBlockData>>&) const;

    void generateLString(char&, std::unordered_map<std::string, std::string>&);
    void generateNotesPool(std::vector<std::vector<AnalogUserInputBlockData>>&);
};