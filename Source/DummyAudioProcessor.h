#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Dummy
{
	class DummyAudioProcessor : public juce::AudioProcessor
	{
	public:
        const juce::String getName() const final
        {
            return "Dummy Audio Processor";
        }

        void prepareToPlay(double, int) final
        {
        }

        void releaseResources() final
        {
        }

        void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) final
        {
        }

        double getTailLengthSeconds() const final
        {
            return 0.0;
        }

        bool acceptsMidi() const final
        {
            return true;
        }

        bool producesMidi() const final
        {
            return true;
        }

        juce::AudioProcessorEditor* createEditor() final
        {
            return new juce::GenericAudioProcessorEditor{ *this };
        }

        bool hasEditor() const final
        {
            return true;
        }

        int getNumPrograms() final
        {
            return 1;
        }

        int getCurrentProgram() final
        {
            return 0;
        }

        void setCurrentProgram(int) final
        {
        }

        const juce::String getProgramName(int) final
        {
            return "";
        }

        void changeProgramName(int, const juce::String&) final
        {
        }

        void getStateInformation(juce::MemoryBlock&) final
        {
        }

        void setStateInformation(const void*, int) final
        {
        }
	};
}