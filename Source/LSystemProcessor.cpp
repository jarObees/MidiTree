#include "LSystemProcessor.h"
#include "Ids.h"
#include "NoteWheel.h"
using namespace AnalogUserInput;
// Map containing illegal strings as keys, and their corrosponding legal string as values.
// // For use in correcting lsys variables and rulesets.
LSystemProcessor::LSystemProcessor(juce::Slider*& _generationsKnob,
                                   Tree::ActiveTreeManager* _activeTreeManager,
                                   juce::Component*& _analogUserInputComponent)
    : generationsKnob(_generationsKnob), activeTreeManager(_activeTreeManager), analogUserInputComponent(_analogUserInputComponent)
{

}


void LSystemProcessor::growLSystem()
{
    DBG("Growing L System");
    if (generationsKnob == nullptr)
        jassertfalse;
    generationsNum = generationsKnob->getValue();
    DBG("Number of Generations: " << generationsNum);
    std::vector<std::vector<AnalogUserInputBlockData>> analogUserInputBlockDataSet
        = makeAnalogUIBlockDataSet();

    auto ruleMap = generateRulemap(analogUserInputBlockDataSet);
    if (ruleMap.size() == 0)
    {
        ///TODO: Create error thrower.
        DBG("YO RULES WRONG!");
    }
    char axiomChar = getAxiomCharFromBDS(analogUserInputBlockDataSet);
    if (axiomChar == '\0')
    {
        ///TODO: Create error thrower
        DBG("NOAXIOM!!!");
    }
    generateLString(axiomChar, ruleMap);
    generateNotesPool(analogUserInputBlockDataSet);
    activeTreeManager->setNotesPool(generatedNotesPool);
}

char LSystemProcessor::getAxiomCharFromBDS(std::vector<std::vector<AnalogUserInputBlockData>>& analogUserInputBlockDataSet) const
{
    for (auto& blockRow : analogUserInputBlockDataSet)
    {
        for (auto& blockData : blockRow)
        {
            if (blockData.axiom == true)
            {
                return blockData.lSysChar;
            }
        }
    }
    return '\0';
}

// Makes a 2D vector of AUIBD in order to manipulate and analyze data in a simple(ish) way.
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
            DBG("row child num: " << child->getNumChildComponents());
            for (auto* rowChild : child->getChildren())
            {
                if (rowChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlock)) //======================= Within InputBlock
                {
                    DBG("Accessing inputBlock =====");
                    DBG("ib child num: " << rowChild->getNumChildComponents());
                    AnalogUserInputBlockData inputBlockData;
                    for (auto* blockChild : rowChild->getChildren())
                    {
                        if (blockChild->getComponentID().startsWith(jiveGui::IdPrefix::inputBlockTop))
                        {
                            DBG("> InputBlockTOP");
                            DBG("ibt child num: " << blockChild->getNumChildComponents());
                            const auto inputBlockChildren = blockChild->getChildren();
                            for (auto* child : inputBlockChildren)
                            {
                                if (child->getComponentID().startsWith(jiveGui::IdPrefix::octavesInput))
                                {
                                    juce::Slider* octavesInput = dynamic_cast<juce::Slider*>(child);
                                    jassert(octavesInput != nullptr);
                                    inputBlockData.octave = octavesInput->getValue();
                                    DBG("oi child num: " << child->getNumChildComponents());
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
                            DBG("nw child num: " << blockChild->getNumChildComponents());
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
                                    DBG("ibb child num: " << blockChild->getNumChildComponents());
                                }
                            }
                        }
                    }
                    if (inputBlockData.noteWheelNum != 0)       // Only pushes stuff that has an actual value.
                        blockRow.push_back(inputBlockData);
                }
            }
            if (blockRow.size() > 1)
                analogUserInputBlockDataSet.push_back(blockRow);
            else
                DBG("Weird aa blockRow. Ignoring...");
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

// Ret
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
    DBG("Generated L String: " << generatedLString.toStdString());
}

// Converts the generated string into a vector<int> representing the interval in half steps from the root note (axiom).
// Example: "3" represents a minor 3rd, so it's converted to 4 (half-steps from the root).
void LSystemProcessor::generateNotesPool(std::vector<std::vector<AnalogUserInputBlockData>>& analogUserInputBlockDataSet)
{
    DBG("Generating notes pool ===========================================================");
    std::string genString = generatedLString.toStdString();
    DBG("Generated L String: " << generatedLString);
    juce::Array<int> notesPlus;
    for (char c : genString)
    {
        bool found = false;
        DBG("Checking: " << c << "in " << genString);
        for (auto& row : analogUserInputBlockDataSet)
        {
            if (found != false)
                break;
            for (auto& blockData : row)
            {
                if (c == blockData.lSysChar)
                {
                    notesPlus.add(blockData.noteWheelNum - 1);
                    DBG("Matched: " << c << "to interval " << (blockData.noteWheelNum - 1));
                    found = true;
                    break;
                }
            }
        }
    }
    DBG("Notes Pool Generated:");
    for (auto& varTing : notesPlus)
    {
        DBG(juce::String(varTing));
    }
    generatedNotesPool = notesPlus;
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
            {
                rightHandSide << blockData.lSysChar;
            }
        }
        DBG("Adding rule: " << leftHandSide << "=" << rightHandSide.str());
        map.emplace(leftHandSide, rightHandSide.str());
    }
    return map;
}