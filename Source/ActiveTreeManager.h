#pragma once
#include "TreeData.h"
namespace Tree
{
	// Is this overkill? Maybe. But I'm learning so whatever.
	class ActiveTreeListener
	{
	public:
		virtual void onActiveTreeChanged(std::shared_ptr<MidiTree> newTree) = 0;
		virtual ~ActiveTreeListener() = default;
	};

	// This class holds the active tree data that is currently being used to generate notes in real-time.
	class ActiveTreeManager
	{
	private:
		std::shared_ptr<MidiTree> activeTree;
		std::vector<ActiveTreeListener*> listeners;
		void sendNotification()
		{
			for (auto* listener : listeners)
			{
				DBG("Sending notification...");
				if (listener)
					listener->onActiveTreeChanged(activeTree);
			}
		}
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
			sendNotification();
		}

		void setNotesPool(juce::Array<int> newNotesPool)
		{
			DBG("Setting new notesPool...");
			activeTree->notesPool = newNotesPool;
			sendNotification();
		}
		void setTreeName(juce::String newName)
		{
			DBG("Setting New TreeName");
			activeTree->name = newName;
			sendNotification();
		}
		juce::Array<int> getNotesPool() const
		{
			return activeTree->notesPool;
		}
		juce::String getTreeName() const
		{
			return activeTree->name;
		}
		bool hasValidTree()
		{
			if (!activeTree->notesPool.isEmpty() && activeTree->name != Tree::MidiTree{}.name)
			{
				return true;
			}
			else
				return false;
		}

		void addListener(ActiveTreeListener* listener)
		{
			DBG("Adding Listener...");
			listeners.push_back(listener);
		}
		void removeListener(ActiveTreeListener* listenerToBeRemoved)
		{
			DBG("Removing Listener...");
			std::erase(listeners, listenerToBeRemoved);
		}
	};
}