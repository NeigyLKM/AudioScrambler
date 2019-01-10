/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioScramblerAudioProcessorEditor::AudioScramblerAudioProcessorEditor (AudioScramblerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	
	bufferslider.setRange(25, 44100*4, 1);
	bufferslider.setValue(processor.tmpBuffersize);
	bufferslider.setTextValueSuffix("Buffer ");
	bufferslider.setCentrePosition(100, 100);
	bufferslider.setSize(200, 100);
	
	addAndMakeVisible(bufferslider);
	
	sliceslider.setRange(25, 44100*2, 1);
	sliceslider.setValue(processor.slicesize);
	sliceslider.setTextValueSuffix("Slice ");
	sliceslider.setCentrePosition(100, 0);
	sliceslider.setSize(200, 100);
	sliceslider.addListener(this);
	addAndMakeVisible(sliceslider);
	
	
	modeButton.setButtonText ("Reverse");
	modeButton.onClick = [this] { modeButtonClicked(); };
//modeButton.setSize (300, 100);
	addAndMakeVisible (modeButton);
}

AudioScramblerAudioProcessorEditor::~AudioScramblerAudioProcessorEditor()
{
}

//==============================================================================
void AudioScramblerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll (Colours::black);
	g.setColour (Colours::white);
	g.setFont(15.0f);

}

void AudioScramblerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	sliceslider.setBounds(40, 40, 100, 20);
	bufferslider.setBounds(20, 20, 100, 20);
	modeButton.setBounds(Rectangle<int>(Point<int>(300,50),Point<int>(400,150)));
}

void AudioScramblerAudioProcessorEditor::modeButtonClicked(){
	
	if (processor.mode == 0){
		processor.mode = 1;
		
	}
	else{
		processor.mode = 0;
	}
	
	
}

void AudioScramblerAudioProcessorEditor::sliderValueChanged (Slider* slider){
	
	double value = slider->getValue();
	if (slider == &bufferslider)
		this->processor.tmpBuffersize = value;
	
	if (slider == &sliceslider)
		this->processor.slicesize = value;
	
	if (this->processor.slicesize > this->processor.tmpBuffersize){
		this->processor.slicesize = this->processor.tmpBuffersize;
		bufferslider.setValue(this->processor.slicesize);
		
		}
	
	
}
