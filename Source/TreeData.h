#pragma once
#include <JuceHeader.h>

namespace Tree
{
	struct MidiTree
	{
		juce::String name = "[Unknown Tree]";
		juce::Array<int> notesPool;
	};
}