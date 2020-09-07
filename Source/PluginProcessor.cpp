//PluginProcessor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessor::ReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor(BusesProperties()
             #if ! JucePlugin_IsMidiEffect
              #if ! JucePlugin_IsSynth
               .withInput("Input",  juce::AudioChannelSet::stereo(), true)
              #endif
               .withOutput("Output", juce::AudioChannelSet::stereo(), true)
             #endif
               )
#endif
, valueTreeState(*this, nullptr, "Parameters", CreateParameters())
{
}

ReverbAudioProcessor::~ReverbAudioProcessor()
{
}

const juce::String ReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int ReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ReverbAudioProcessor::getProgramName(int index)
{
    return {};
}

void ReverbAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void ReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

}

void ReverbAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void ReverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    AddGainProcessing(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

bool ReverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* ReverbAudioProcessor::createEditor()
{
    return new ReverbAudioProcessorEditor (*this);
}

void ReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream(destData, true).writeFloat(*valueTreeState.getRawParameterValue("gainID"));
}

void ReverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    *valueTreeState.getRawParameterValue("gainID") = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
}

juce::AudioProcessorValueTreeState &ReverbAudioProcessor::GetValueTreeState()
{
    return valueTreeState;
}

const juce::AudioProcessorValueTreeState::ParameterLayout ReverbAudioProcessor::CreateParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("gainID", // parameterID
                                                                     "Gain",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 3.0f, 0.01f, 0.5f), // min, max, stepsize, skew factor
                                                                     1.0f));  // default value)
    
    return { parameters.begin(), parameters.end() };
}

void ReverbAudioProcessor::AddGainProcessing(juce::AudioBuffer<float>& buffer)
{
    float currentGainValue = *valueTreeState.getRawParameterValue("gainID");

    buffer.applyGain(currentGainValue);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}
