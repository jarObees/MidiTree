#pragma once
#include <JuceHeader.h>

namespace Tree
{
	// ALWAYS INSTANTIATE BOTH VARIABLES
	struct MidiTree
	{
		juce::String name = "[Unsaved Tree]";
		juce::Array<int> notesPool;
	};
}