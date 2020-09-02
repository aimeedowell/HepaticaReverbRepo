//PluginProcessor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

PitchShiftAudioProcessor::PitchShiftAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PitchShiftAudioProcessor::~PitchShiftAudioProcessor()
{
}

const juce::String PitchShiftAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PitchShiftAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PitchShiftAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PitchShiftAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PitchShiftAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchShiftAudioProcessor::getNumPrograms()
{
    return 1;
}

int PitchShiftAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchShiftAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PitchShiftAudioProcessor::getProgramName (int index)
{
    return {};
}

void PitchShiftAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void PitchShiftAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void PitchShiftAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PitchShiftAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void PitchShiftAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

bool PitchShiftAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PitchShiftAudioProcessor::createEditor()
{
    return new PitchShiftAudioProcessorEditor (*this);
}

void PitchShiftAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void PitchShiftAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PitchShiftAudioProcessor();
}
