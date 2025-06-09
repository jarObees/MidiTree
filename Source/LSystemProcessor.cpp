#include "LSystemProcessor.h"
#include "Ids.h"
#include "NoteWheel.h"
using namespace AnalogUserInput;
// Map containing illegal strings as keys, and their corrosponding legal string as values.
// // For use in correcting lsys variables and rulesets.
LSystemProcessor::LSystemProcessor(juce::Slider*& generationsKnob, 
                                   juce::Value& _rulesetUserInput, 
                                   juce::Value& _axiomUserInput, 
                                   juce::Value& _generatedLString,
                                   juce::Value& _notesPool,
                                   juce::Component*& analogUserInputComponent)
    : rulesetUserInput(_rulesetUserInput), 
    axiomUserInput(_axiomUserInput), 
    generatedLString(_generatedLString), 
    generationsKnob(generationsKnob), 
    notesPool(_notesPool),
    analogUserInputComponent(analogUserInputComponent)
{
        
}

/// TODO: Refactor to use the current state of the UI. 
/// Pass in a reference to the parent analog user input component. Go thruogh da shits and analyze.
void LSystemProcessor::growLSystem()
{
    DBG("Growing L System");
    if (generationsKnob == nullptr)
        jassertfalse;
    generationsNum = generationsKnob->getValue();
    DBG("Number of Generations: " << generationsNum);
    //if (rulesetUserInput.getValue().isVoid() || axiomUserInput.getValue().isVoid())
    //{
    //    DBG("SHITS EMPTY");
    //    return;
    //}
    std::vector<std::vector<AnalogUserInputBlockData>> analogUserInputBlockDataSet
        = makeAnalogUIBlockDataSet();



    auto ruleMap = generateRulemap(analogUserInputBlockDataSet);
    char axiomChar = getAxiomCharFromBDS(analogUserInputBlockDataSet);
    generateLString(axiomChar, ruleMap);
}

char LSystemProcessor::getAxiomCharFromBDS(std::vector<std::vector<AnalogUserInputBlockData>>& analogUserInputBlockDataSet) const
{
    for (auto& blockRow : analogUserInputBlockDataSet)
    {
        for (auto& blockData : blockRow)
        {
            if (blockData.axiom == true)
            {
                ///TODO: Set the value axiom thing to this as well if possible.
                return blockData.lSysChar;
            }
        }
    }
}

std::vector<std::vector<AnalogUserInputBlockData>> LSystemProcessor::makeAnalogUIBlockDataSet()
{
    jassert(analogUserInputComponent != nullptr);
    std::vector<std::vector<AnalogUserInputBlockData>> analogUserInputBlockDataSet;
    for (auto* child : analogUserInputComponent->getChildren())
    {
        // Access Rows
        if (child->getComponentID().startsWith(jiveGui::IdPrefix::inputRow))
        {
            std::vector<AnalogUserInputBlockData> blockRow;
            DBG("Accessed row...");
            for (auto* rowChild : child->getChildren())
            {
                if (rowChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlock)) //======================= Within InputBlock
                {
                    DBG("Accessing inputBlock =====");
                    AnalogUserInputBlockData inputBlockData;
                    for (auto* blockChild : rowChild->getChildren())
                    {
                        if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockTop))
                        {
                            DBG("> InputBlockTOP");
                            const auto inputBlockChildren = blockChild->getChildren();
                            for (auto* child : inputBlockChildren)
                            {
                                if (child->getComponentID().startsWith(jiveGui::IdPrefix::octavesInput))
                                {
                                    juce::Slider* octavesInput = dynamic_cast<juce::Slider*>(child);
                                    jassert(octavesInput != nullptr);
                                    inputBlockData.octave = octavesInput->getValue();
                                    DBG(">> Octaves Input: " << inputBlockData.octave);
                                }
                                if (child->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockAxiom))
                                {
                                    juce::Button* axiomInput = dynamic_cast<juce::Button*>(child);
                                    jassert(axiomInput != nullptr);
                                    inputBlockData.axiom = axiomInput->getToggleState();
                                }
                                juce::String dbgButtonState = (inputBlockData.axiom == false) ? "false" : "true";
                                DBG(">> Axiom: " << dbgButtonState);
                            }
                        }
                        else if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::noteWheel))
                        {
                            DBG("> NoteWheel");
                            // Found note wheel.
                            juce::Slider* noteWheelInput = dynamic_cast<juce::Slider*>(blockChild);
                            jassert(noteWheelInput != nullptr);
                            inputBlockData.noteWheelNum = noteWheelInput->getValue();
                            DBG(">> Notewheel Num: " << inputBlockData.noteWheelNum);
                        }
                        else if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockBottom))
                        {
                            DBG("> InputBlockBOT");
                            const auto inputBlockChildren = blockChild->getChildren();
                            for (auto* child : inputBlockChildren)
                            {
                                if (child->getComponentID().startsWith(jiveGui::IdPrefix::directionInput))
                                {
                                    juce::Slider* directionInput = dynamic_cast<juce::Slider*>(child);
                                    jassert(directionInput != nullptr);
                                    inputBlockData.ascending = (directionInput->getValue() == 0) ? true : false;
                                    juce::String dbgDirection = (inputBlockData.ascending == true) ? "Ascending" : "Descending";
                                    DBG(">> Direction: " << dbgDirection);
                                }
                            }
                        }

                    }
                    if (inputBlockData.noteWheelNum != 0)       // Only pushes stuff that has an actual value.
                        blockRow.push_back(inputBlockData);
                }
            }
            if (blockRow.size() != 0)
                analogUserInputBlockDataSet.push_back(blockRow);
        }
    }
	setBlockDataSetLSysChars(analogUserInputBlockDataSet);
    return analogUserInputBlockDataSet;
}

// Goes through and sets the lSysChar for each block in the blockDataSet.
void LSystemProcessor::setBlockDataSetLSysChars(std::vector<std::vector<AnalogUserInputBlockData>>& blockDataSet)
{
    char asciiChar = 'A';
    std::map<std::tuple<bool, int, int>, char> uniqueInputs; // Pairs the unique combination of vars with a char.
    int i = 0;
    for (auto& blockRow : blockDataSet)
    {
        DBG("Row: " << i);
        i++;
        int j = -1;
        for (auto& blockData : blockRow)
        {
            DBG("Block: " << j);
            j++;

            if (asciiChar > 'z')
            {
                // Throw Error
                jassertfalse;
            }
            auto key = std::make_tuple(blockData.ascending,
                                       blockData.noteWheelNum,
                                       blockData.octave);
            auto it = uniqueInputs.find(key);
            if (it != uniqueInputs.end())
            {
                blockData.lSysChar = it->second;
                DBG("Predecessor found. Setting this block's lSysChar to: " << blockData.lSysChar);
            }
            else
            {
                blockData.lSysChar = asciiChar;
                uniqueInputs[key] = asciiChar;
                DBG("New note entry. Setting lSysChar to " << asciiChar);
                asciiChar++;
            }
        }
    }
}

void LSystemProcessor::generateLString(char& axiomChar, std::unordered_map<std::string, std::string>& ruleMap)
{
    DBG("GENERATING L STRING ===============================================");
    std::string genString = std::string{axiomChar};
    // For each generation...
    for (int i = 0; i < generationsNum; ++i)
    {
        // Search through each character and check if it needs to convert to new character and do so accordingly.
        std::ostringstream nextGen;
        DBG("Gen: " << i << "----------");
        for (char c : genString)
        {
            DBG("Current genString: " << genString);
            DBG("Checking: " << c);
            if (ruleMap.find(std::string{ c }) != ruleMap.end())
            {
                DBG("Found " << c << " in ruleMap. Replacing with: " << ruleMap[std::string{c}]);
                nextGen << ruleMap.at(std::string{c});
            }
            else
            {
                DBG("Could not find " << c << " in ruleMap.");
                DBG("Adding: " << c);
                nextGen << c;
            }
        }
        genString = std::move(nextGen.str());
    }
    generatedLString = juce::String(genString);
    DBG("Generated L String: " << generatedLString.getValue().toString());
}

// Converts the generated string into a vector<int> representing the interval in half steps from the root note (axiom).
// Example: "3" represents a minor 3rd, so it's converted to 4 (half-steps from the root).
void LSystemProcessor::generateNotesPool()
{
    DBG("Generating notes pool ===========================================================");
    std::string genString = generatedLString.toString().toStdString();
    juce::Array<juce::var> notesPlus;
    for (char c : genString)
    {
        DBG("Char: " << c);
        std::string s{ c };
        auto it = replacementRulesToInt.find(s);
        if (it != replacementRulesToInt.end())
        {
            DBG("Adding " << it->second);
            notesPlus.add(it->second);
        }
        else
        {
            DBG("NOTES POOL: REPLACEMENT RULES AND L STRING MISMATCH");
            return;
        }
    }
    DBG("Notes Pool Generated!");
    notesPool = notesPlus;
}

// Checks if the ruleset is valid, and if so, populates currentLSystemVariables and currentLSystemRules.
bool LSystemProcessor::setLSystemRulesAndVariables()
{
    DBG("Setting L System Rules and Variables");

    DBG("rulesetUserInput" << rulesetUserInput.toString().toStdString());
    // Iterates through and verifies each line in ruleSetInput.
    std::stringstream ss(rulesetUserInput.toString().toStdString());
    std::string line;
    juce::SortedSet<std::string> tempLsysVariables;
    juce::SortedSet<std::string> tempLsysRulesets;
    while (std::getline(ss, line))
    {
        // If the line is valid, adds it to the sorted set.
        if (std::regex_match(line, rulesetPattern))
        {
            tempLsysRulesets.add(line);
            auto begin = std::sregex_iterator(line.begin(), line.end(), variablePattern);
            auto end = std::sregex_iterator();
            for (auto it = begin; it != end; ++it)
            {
                tempLsysVariables.add(it->str());
            }
        }
        else
        {
            DBG("Incorrect ruleset format");
            // throwCustomError("Incorrect Ruleset Format.");
            return false;
        }
    }
    DBG("Everything's good, properties set!");
    currentLSystemVariables = tempLsysVariables;
    currentLSystemRules = tempLsysRulesets;
    return true;
}

// Checks that axiom is valid.
bool LSystemProcessor::confirmAxiom()
{
    DBG("Checking axiom correctness...");
    DBG("l SYS VAR SIZE: " << currentLSystemVariables.size());
    for (auto it = currentLSystemVariables.begin(); it != currentLSystemVariables.end(); ++it)
    {
        DBG("Checking: " << axiomUserInput.toString().toStdString() << "against: " << *it);
        if (*it == axiomUserInput.toString().toStdString())
        {
            return true;
        }
    }
    DBG("Axiom is not a valid variable!");
    return false;
}



// Main function to generate and run everything necessary to produce an L system.
void LSystemProcessor::generateLSystem(const uint8_t& gens)
{
    translateSet(currentLSystemRules);
    translateSet(currentLSystemVariables);
    // std::unordered_map<std::string, std::string> newLsysRulemap = generateRulemap();
   // LSystem lsystem(lsysAxiom, newLsysRulemap);
    //lsystem.generate(generationsNum);
    // lSystems.push_back(lsystem);

    //FOR TESTING PURPOSES=============================================================
    const LSystem& testLSystem = lSystems[0];
    // notesPool = generateNotesPool(testLSystem.genString);
}

// We need to verify that all variables in l system are single characters for future rule application.
// Searches through each string in the set and replaces illegal substrings with legal ones using the replacementRules map.
void LSystemProcessor::translateSet(juce::SortedSet<std::string>& stringSet)
{
    juce::SortedSet<std::string> translatedRuleset;
    
    for (const std::string& ruleLine : stringSet) // Iterates through each line in the ruleset.
    {
        std::string newRuleLine = ruleLine;
        for (const auto& [key, value] : replacementRulesToChar)
        {
            size_t pos = 0;
            while ((pos = newRuleLine.find(key, pos)) != std::string::npos) // Replace any instance of the key (illegal string) in the ruleLine with the value (legal string)
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
std::unordered_map<std::string, std::string> LSystemProcessor::generateRulemap(std::vector<std::vector<AnalogUserInputBlockData>>& inputBlockDataSet)
{
    DBG("Generating Ruleset");
    std::unordered_map<std::string, std::string> map;
    int i = 0;
    for (auto& blockRow : inputBlockDataSet)
    {
        DBG("Row: " << i);
        i++;
        bool firstInput = true;
        std::stringstream rightHandSide;
        std::string leftHandSide;
        for (auto& blockData : blockRow)
        {
            if (firstInput)
            {
                leftHandSide = std::string{ blockData.lSysChar };
                firstInput = false;
            }
            else
                rightHandSide << blockData.lSysChar;
        }
        DBG("Adding rule: " << leftHandSide << "=" << rightHandSide.str());
        map.emplace(leftHandSide, rightHandSide.str());
    }
    return map;
}