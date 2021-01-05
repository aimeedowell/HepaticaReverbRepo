/*
  ==============================================================================

    CustomReverb.h
    Created: 3 Jan 2021 1:03:16pm
    Author:  Aimee Dowell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "CombFilter.h"
#include "AllPassFilter.h"

using namespace juce;

class ShroederReverb
{
public:
    ShroederReverb(juce::AudioProcessorValueTreeState &valueTreeState);

    ~ShroederReverb();
    
    void prepareToPlay(const double sampleRate, int samplesPerBlock);

    void reset();
    
    void process(juce::AudioBuffer<float> &buffer) noexcept;

private:
    
    void UpdateDamping() noexcept;

    void SetDamping (const float dampingToUse, const float roomSizeToUse) noexcept;
    
    void TuneFilters(int stereoSpread, double sampleRate);
    
    void SetCoefficients();
    
    void UpdateFilters();
    
    static const int numCombs = 4, numAllPasses = 2, numChannels = 2;
    constexpr static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
    constexpr static const short allPassTunings[] = { 556, 441, 341, 225 };
    constexpr static const short earlyReflectTunings[] = { 56, 41, 31, 25, 10, 5, 3};

    CombFilter comb [numChannels][numCombs]; //create comb for each channel
    AllPassFilter allPass [numChannels][numAllPasses]; //create allpass for each channel
    
    AllPassFilter earlyReflections [numChannels][numAllPasses]; //create allpass for each channel

    using Filter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

    juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter> processorChain;
    
    std::unique_ptr<juce::dsp::DryWetMixer<float>> mixer;
    
    SmoothedValue<float> damping, feedback, delay, earlyReflectFeedback;
    
    juce::AudioProcessorValueTreeState &treeParameters;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::None> delayLine{44100};
    
    float gain = 0.015f;
    double globeSampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShroederReverb)
};


