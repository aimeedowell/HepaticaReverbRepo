//PluginProcessor.h

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
    
    void CreateCombFilters();
    
    void SetDelayValues(double sampleRate);
    void SetRateValues();
    
    juce::AudioProcessorValueTreeState valueTreeState;
    
    std::vector<float> bufferAmplitude;
    
    std::unique_ptr<ShroederReverb> myReverb;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessor)
};
