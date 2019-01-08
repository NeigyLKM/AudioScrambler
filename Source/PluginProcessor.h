/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <iostream>
using namespace std;
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class AudioScramblerAudioProcessor  : public AudioProcessor
{
public:
	
	int tmpBuffersize = 44100. *2;
	int slicelength = 0;
	int slicesize = 22050;
	
	
    //==============================================================================
    AudioScramblerAudioProcessor();
    ~AudioScramblerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	
	int slicecounter = 0;
	AudioSampleBuffer tmpBuffer;
	AudioSampleBuffer tmpBufferContinuous;
	int countercontinuous =0;
	int randompos = 0;
	int tmpBufferContinuoussize = 44100. *2;
	int playslices = 0;
	float slope;
	int smoothinglenght = 0.03 * 44100;
	int smoothinglengthEnd = 0;
	float targetvalueBegin = 0;
	float targetvalueEnd= 0;
	

	

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioScramblerAudioProcessor)
};
