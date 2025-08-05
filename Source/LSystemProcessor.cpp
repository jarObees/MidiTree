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

void LSystemProcessor::connectWithEditor(jive::GuiItem* editor)
{
	mainEditor = editor;
	jassert(mainEditor != nullptr);
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

// Makes a 2D vector of AUI Block Data in order to manipulate and analyze data in a simple(ish) way.
std::vector<std::vector<AnalogUserInputBlockData>> LSystemProcessor::makeAnalogUIBlockDataSet()
{
    jassert(analogUserInputComponent != nullptr);
	jassert(mainEditor != nullptr);
    std::vector<std::vector<AnalogUserInputBlockData>> analogUserInputBlockDataSet;

    // Loop through all combos of row/col
    for (int row = 0; row < jiveGui::AnalogUserInput::NUMBLOCKROWS; row++)
    {
        std::vector<AnalogUserInputBlockData> rowData;
        for (int col = -1; col < jiveGui::AnalogUserInput::NUMBLOCKCOLUMNS; col++)
        {
            AnalogUserInputBlockData blockData;

            // Find NW Value
            const auto jiveNoteWheelId = jiveGui::idRowColMaker(jiveGui::IdPrefix::noteWheel, row, col);
            auto* noteWheel = dynamic_cast<juce::Slider*>(jive::findItemWithID(*mainEditor, jiveNoteWheelId)->getComponent().get());
            jassert(noteWheel != nullptr);
            blockData.noteWheelNum = noteWheel->getValue();
            if (blockData.noteWheelNum == 0)                // If it's empty, then no need to store data.
                continue;

            // Find Octaves Value
			const auto jiveOctavesId = jiveGui::idRowColMaker(jiveGui::IdPrefix::octavesInput, row, col);
			auto* octavesInput = dynamic_cast<juce::Slider*>(jive::findItemWithID(*mainEditor, jiveOctavesId)->getComponent().get());
			jassert(octavesInput != nullptr);
			blockData.octave = octavesInput->getValue();

            // Find Direction
			const auto jiveDirectionId = jiveGui::idRowColMaker(jiveGui::IdPrefix::directionInput, row, col);
			auto* directionInput = dynamic_cast<juce::Button*>(jive::findItemWithID(*mainEditor, jiveDirectionId)->getComponent().get());
			jassert(directionInput != nullptr);
            blockData.ascending = directionInput->getToggleState();

            // Find if axiom
            if (col == -1)
            {
                const auto jiveAxiomId = jiveGui::idRowColMaker(jiveGui::IdPrefix::axiomToggle, row, col);
                auto* axiomInput = dynamic_cast<juce::Button*>(jive::findItemWithID(*mainEditor, jiveAxiomId)->getComponent().get());
                jassert(axiomInput != nullptr);
                blockData.axiom = axiomInput->getToggleState();
            }

            DBG("Data Block:: Octaves = " << blockData.octave 
                << "| NW =" << blockData.noteWheelNum 
                << "Ascending? = " << (blockData.ascending ? "true" : "false")
                << "Axiom? =" << (blockData.axiom ? "true" : "false"));
            rowData.push_back(blockData);
        }
        if (rowData.size() > 1)
            analogUserInputBlockDataSet.push_back(rowData);
        else
            DBG("Incomplete row. Skipping...");
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