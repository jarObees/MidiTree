#pragma once
#include "TreeData.h"
namespace Tree
{
	// This class holds the active tree data that is currently being used to generate notes in real-time.
	class ActiveTreeManager
	{
	private:
		std::shared_ptr<MidiTree> activeTree;
	public:
		ActiveTreeManager() 
			: activeTree(std::make_shared<MidiTree>()) 
		{};

		// You can pass in a new MidiTree to be set as the new activeTree.
		void swapActiveTree(std::shared_ptr<MidiTree> newTree)
		{
			if (newTree)
			{
				DBG("Swapping activeTree...");
				activeTree = newTree;
			}
			else
				jassertfalse;
		}
		// Returns the current activeTree. Usually so you can modify the MidiTree.
		std::shared_ptr<MidiTree> getActiveTree() const
		{
			return activeTree;
		}
	};
}