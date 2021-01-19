//PluginProcessor.h

/*
 
 This class is the main class for the plugins, it controls the processing and creates the plugin as a whole.
 
 A Custom Reverb class is initialised and this class calls on its prepareToPlay and process methods. The Reverb class is foward declared to reduce compile times, and is wrapped in a unique pointer (smart pointer) which controls memory management of the object to eliminate bad handling of 'new' and 'delete' variables, which can cause memory leaks.
 
 The output of each block is passed into a small buffer which the audio visualiser class uses to predict the height of the meter.
 
 Parameters of the AudioProcessorValueTreeState are initalised here with default values, min/max values and skew factor. The IDs of these parameters are used in Custom Phaser to get the raw values of each control parameter for the phaser.
 
 */

#pragma once

#include <JuceHeader.h>
class ShroederReverb;

class ReverbAudioProcessor
    : public juce::AudioProcessor
{
public:
    ReverbAudioProcessor();
    
    //juce::AudioProcessor
    ~ReverbAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    
    bool producesMidi() const override;
    
    bool isMidiEffect() const override;
    
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    
    int getCurrentProgram() override;
    
    void setCurrentProgram(int index) override;
    
    const juce::String getProgramName(int index) override;
    
    void changeProgramName(int index, const juce::String& newName) override;
    
    void getStateInformation(juce::MemoryBlock& destData) override;
    
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState &GetValueTreeState();
    
    void AddGainProcessing(juce::AudioBuffer<float>& buffer);
    
    std::vector<float> &GetAmplitudeLevel();

private:
    const juce::AudioProcessorValueTreeState::ParameterLayout CreateParameters();
    
    juce::AudioProcessorValueTreeState valueTreeState;
    
    std::vector<float> bufferAmplitude;
    
    std::unique_ptr<ShroederReverb> myReverb;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessor)
};
