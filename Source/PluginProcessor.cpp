//PluginProcessor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ShroederReverb.h"

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
, bufferAmplitude({0.0f, 0.0f})
, myReverb(std::make_unique<ShroederReverb>(valueTreeState))
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
    myReverb->prepareToPlay(sampleRate, samplesPerBlock);
}

void ReverbAudioProcessor::releaseResources()
{
    myReverb->reset();
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
    
    myReverb->process(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

        if (channel == 0)
            bufferAmplitude[0] = buffer.getMagnitude(channel, 0, buffer.getNumSamples());
        else if (channel == 1)
            bufferAmplitude[1] = buffer.getMagnitude(channel, 0, buffer.getNumSamples());
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
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("panningID", // parameterID
                                                                     "Panning",   // parameter name
                                                                     juce::NormalisableRange<float>(-100.0f, 100.0f, 1.0f, 1.0f), // min, max, stepsize
                                                                     0.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("wetDryID", // parameterID
                                                                     "Mix",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), // min, max, stepsize
                                                                     50.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("reverbSizeID", // parameterID
                                                                     "Reverb Size",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f), // min, max, stepsize, skew factor
                                                                     50.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("preDelayID", // parameterID
                                                                     "Pre-Delay",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 20.f, 1.0f, 0.5f), // min, max, stepsize, skew factor
                                                                     5.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("earlyReflectionsID", // parameterID
                                                                     "Early Reflections",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 99.0f, 1.0f), // min, max, stepsize
                                                                     50.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("stereoSpreadID", // parameterID
                                                                     "Stereo Spread",   // parameter name
                                                                     juce::NormalisableRange<float>(0.5f, 1.5f, 0.1f), // min, max, stepsize
                                                                     1.0f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("dampingID", // parameterID
                                                                     "Damping",   // parameter name
                                                                     juce::NormalisableRange<float>(0.0f, 100.f, 1.f), // min, max, stepsize
                                                                     50.f));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("noEQID", // parameterID
                                                                    "EQ off",   // parameter name
                                                                    true));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("lowPassID", // parameterID
                                                                    "Low Pass",   // parameter name
                                                                    false));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("highPassID", // parameterID
                                                                    "High Pass",   // parameter name
                                                                    false));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("lowShelfID", // parameterID
                                                                    "Low Shelf",   // parameter name
                                                                    false));  // default value)
    parameters.push_back(std::make_unique<juce::AudioParameterBool>("highShelfID", // parameterID
                                                                    "High Shelf",   // parameter name
                                                                    false));  // default value)
    
    return { parameters.begin(), parameters.end() };
}

void ReverbAudioProcessor::AddGainProcessing(juce::AudioBuffer<float>& buffer)
{
    float currentGainValue = *valueTreeState.getRawParameterValue("gainID");

    buffer.applyGain(currentGainValue);
}

std::vector<float> &ReverbAudioProcessor::GetAmplitudeLevel()
{
    return bufferAmplitude;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbAudioProcessor();
}
