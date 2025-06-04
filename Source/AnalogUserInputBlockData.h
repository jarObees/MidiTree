#pragma once
// This class will holds data from each input block and is to be used in the LSystemManager.
namespace AnalogUserInput
{
	class AnalogUserInputBlockData
	{
	private:
	public:
		bool axiom	      = false;
		bool ascending    = false;
		std::string note  = "";
		int octave        = 0;
		int intervalNum   = 0;
		char id           = '\0';

		bool isValid() const
		{
			if (id == '\0')
				return false;
			else
				return true;
		}
	};
}