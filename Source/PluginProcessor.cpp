/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioScramblerAudioProcessor::AudioScramblerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AudioScramblerAudioProcessor::~AudioScramblerAudioProcessor()
{
}

//==============================================================================
const String AudioScramblerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioScramblerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioScramblerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioScramblerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioScramblerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioScramblerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioScramblerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioScramblerAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioScramblerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioScramblerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioScramblerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	
	tmpBuffer.setSize(2, tmpBuffersize);
	tmpBufferContinuous.setSize(2, tmpBufferContinuoussize);
	
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioScramblerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioScramblerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioScramblerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    auto totalNumInputChannels  = getTotalNumInputChannels();
	const int bufferlength = buffer.getNumSamples();
	
	if (slicesize > tmpBuffersize)
		slicesize = tmpBuffersize - 20;
	
	if (slicelength == 0){
		slicelength = rand()%((slicesize));

	if (slicecounter == 0)
		randompos = rand()%( tmpBuffersize - slicelength - bufferlength);
	}

	//loop over channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {

		//fill the continuous audio buffer
		if (countercontinuous + bufferlength < tmpBufferContinuoussize){
			tmpBufferContinuous.copyFrom(channel, countercontinuous , buffer, channel, 0, bufferlength);
			
		}
		else{
			const int remainingcontinuous =  countercontinuous + bufferlength - tmpBufferContinuoussize;
			tmpBufferContinuous.copyFrom(channel, countercontinuous , buffer, channel, 0, bufferlength -  remainingcontinuous);
			tmpBufferContinuous.copyFrom(channel, 0 , buffer, channel, bufferlength - remainingcontinuous, remainingcontinuous);
			
			playslices = 1;
			
			//once the continuous buffer is full, copy it into the tmpbuffer
			tmpBuffer.makeCopyOf(tmpBufferContinuous);
			for (int channel = 0; channel < totalNumInputChannels; ++channel)
				tmpBuffer.reverse(channel, 0, tmpBuffersize);
			}
		
		//plays the slices
		if (playslices)
			{
			if(slicecounter < slicelength){
				
				buffer.copyFrom(channel, 0, tmpBuffer, 0, randompos + slicecounter , bufferlength);
				
				//cure the clipping only at the beginning of the slice
				if (slicecounter == 0 and smoothinglenght < std::ceil(slicelength/4.)){
					auto* channelData = buffer.getWritePointer (channel);
					smoothinglenght = ceil(slicelength/5.);
					slope = (channelData[smoothinglenght] - targetvalueEnd)/smoothinglenght;
					for (int i =0; i<smoothinglenght; ++i)
						{
							channelData[i] = targetvalueEnd + slope*i;
						}
					//targetvalueBegin = channelData[0];
					//for (int i = smoothinglenght; i > 0; --i)
					//	{
					//	auto newGain = gainSmoother.getNextValue();
					//	channelData[i] *= newGain;
						
					//	}
					}
				}
			else{
				const int remaining2 = slicecounter - slicelength;
				buffer.copyFrom(channel, 0, tmpBuffer, 0, randompos + slicecounter , bufferlength - remaining2);
				auto* channelData = buffer.getReadPointer (channel);
				targetvalueEnd = channelData[bufferlength];


			}
		
		}
	}
	//normalize the buffer
//	for (int channel = 0; channel < totalNumInputChannels; ++channel){
//		auto* channelData = buffer.getWritePointer (channel);
//		float highNum = buffer.getMagnitude(channel, 0, bufferlength);
//		for (auto sample = 0; sample< bufferlength; ++sample)
//			channelData[sample] = channelData[sample] / highNum;
//	}
	
	//keeps track of the sample index, reset to 0 when > tmpbuffer
	countercontinuous += bufferlength;
	slicecounter += bufferlength;
	
	if (slicecounter > slicelength){
		slicecounter = 0;
		slicelength = 0;
	}
	
	if (countercontinuous > tmpBufferContinuoussize)
		countercontinuous = 0;
	
	//counter %= tmpbufferlength;
}

//==============================================================================
bool AudioScramblerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioScramblerAudioProcessor::createEditor()
{
    return new AudioScramblerAudioProcessorEditor (*this);
}

//==============================================================================
void AudioScramblerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioScramblerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioScramblerAudioProcessor();
}
