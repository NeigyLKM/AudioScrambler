/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioScramblerAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    AudioScramblerAudioProcessorEditor (AudioScramblerAudioProcessor&);
    ~AudioScramblerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	
	void sliderValueChanged (Slider* slider) override;
	void modeButtonClicked();


private:
	
	

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	AudioScramblerAudioProcessor& processor;
	
	Slider bufferslider;
	Slider sliceslider;
	ToggleButton modeButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioScramblerAudioProcessorEditor)
};
