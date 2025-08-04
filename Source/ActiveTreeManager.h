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

		void setActiveTree(std::shared_ptr<MidiTree> newTree)
		{
			if (newTree)
			{
				DBG("Swapping activeTree...");
				activeTree = newTree;
			}
			else
				jassertfalse;
		}
		std::shared_ptr<MidiTree> getActiveTree() const
		{
			return activeTree;
		}
	};
}