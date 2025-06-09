#pragma once
// This class will holds data from each input block and is to be used in the LSystemManager.
namespace AnalogUserInput
{
	class AnalogUserInputBlockData
	{
	private:
	public:
		bool axiom = false;
		bool ascending = false;
		int noteWheelNum = 0;			// Num represents index in noteWheel. num - 1 is the actual half-step num interval it represents.
		int octave = 0;

		int intervalNum = 0;			// For use in notePool
		char lSysChar = '\0';			// For use in l system generation
		std::string noteName = "";

		bool isValid() const
		{
			if (lSysChar == '\0')
				return false;
			else
				return true;
		}

		bool operator==(const AnalogUserInputBlockData& other) const
		{
			if (axiom == other.axiom &&
				ascending == other.ascending &&
				noteWheelNum == other.noteWheelNum &&
				octave == other.octave
				)
				return true;
			else
				return false;
		}
	};
}