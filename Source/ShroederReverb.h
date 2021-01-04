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
// shroeders reverb
class ShroederReverb
{
public:
    //==============================================================================
    ShroederReverb(juce::AudioProcessorValueTreeState &valueTreeState)
    : treeParameters(valueTreeState)
    {
        prepareToPlay(44100.0, 100);
        updateDamping();
    }

    void prepareToPlay(const double sampleRate, int samplesPerBlock)
    {
        jassert (sampleRate > 0);

        static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
        static const short allPassTunings[] = { 556, 441, 341, 225 };
        const int stereoSpread = 23;

        for (int i = 0; i < numCombs; ++i)
        {
            comb[0][i].setSize (((int)sampleRate * (combTunings[i] - stereoSpread)) / 44100);
            comb[1][i].setSize (((int)sampleRate * (combTunings[i] + stereoSpread)) / 44100);
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[0][i].setSize (((int)sampleRate * (allPassTunings[i] - stereoSpread)) / 44100);
            allPass[1][i].setSize (((int)sampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
        }

        const double smoothTime = 0.01;
        damping .reset (sampleRate, smoothTime);
        feedback.reset (sampleRate, smoothTime);
        
        dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = numChannels;
        
        delayLine.reset();
        delayLine.prepare(spec);
        
        float preDelay = *treeParameters.getRawParameterValue("preDelayID") * 1000;
        delay.setTargetValue(preDelay);
        
        delayLine.setDelay(delay.getNextValue());
    }

    void reset()
    {
        for (int j = 0; j < numChannels; ++j)
        {
            for (int i = 0; i < numCombs; ++i)
                comb[j][i].clear();

            for (int i = 0; i < numAllPasses; ++i)
                allPass[j][i].clear();
        }
    }

    void process(juce::AudioBuffer<float>& buffer) noexcept
    {
        auto* left = buffer.getWritePointer(0);
        auto* right = buffer.getWritePointer(1);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float input = (left[i] + right[i]) * gain;
            float wetL = 0, wetR = 0;
            
            updateDamping();
            
            const float damp    = damping.getNextValue();
            const float feedbck = feedback.getNextValue();

            for (int j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
            {
                wetL += comb[0][j].process (input, damp, feedbck);
                wetR += comb[1][j].process (input, damp, feedbck);
            }

            for (int j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
            {
                wetL = allPass[0][j].process (wetL);
                wetR = allPass[1][j].process (wetR);
            }
            float preDelay = *treeParameters.getRawParameterValue("preDelayID") * 1000;
            delay.setTargetValue(preDelay);
            
            delayLine.setDelay(delay.getNextValue());
            delayLine.pushSample(0, wetL);
            delayLine.pushSample(1, wetR);
        
            wetL += delayLine.popSample(0, delay.getNextValue());
            wetR += delayLine.popSample(1, delay.getNextValue());
            
            float panValue = *treeParameters.getRawParameterValue("panningID");
            float panGain = 0.5 + (float(panValue)/200);
            
            auto leftAmp = std::sin((1 - panGain) * (M_PI/2));
            auto rightAmp = std::sin(panGain * (M_PI/2));
                
            wetL *= leftAmp;
            wetR *= rightAmp;
            
            float dryWetLevel = *treeParameters.getRawParameterValue("wetDryID")/100;

            left[i]  = (wetL * dryWetLevel) + (left[i]  * (1 - dryWetLevel));
            right[i] = (wetR * dryWetLevel) + (right[i] * (1 - dryWetLevel));
        }
    }

private:
    
    void updateDamping() noexcept
    {
        const float roomScaleFactor = 0.28f;
        const float roomOffset = 0.7f;
        const float dampScaleFactor = 0.4f;
        
        float roomSizeValue = *treeParameters.getRawParameterValue("reverbSizeID")/100;

        setDamping(damping.getNextValue() * dampScaleFactor,
                        roomSizeValue * roomScaleFactor + roomOffset);
    }

    void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept
    {
        damping.setTargetValue (dampingToUse);
        feedback.setTargetValue (roomSizeToUse);
    }
    
    void TuneFilters(int stereoSpread, double sampleRate)
    {
        for (int i = 0; i < numCombs; ++i)
        {
            comb[0][i].setSize (((int)sampleRate * (combTunings[i])) / 44100);
            comb[1][i].setSize (((int)sampleRate * (combTunings[i] + stereoSpread)) / 44100);
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[0][i].setSize (((int)sampleRate * (allPassTunings[i])) / 44100);
            allPass[1][i].setSize (((int)sampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
        }
    }
    
    static const int numCombs = 4, numAllPasses = 2, numChannels = 2;
    constexpr static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
    constexpr static const short allPassTunings[] = { 556, 441, 341, 225 };

    CombFilter comb [numChannels][numCombs]; //create comb for each channel
    AllPassFilter allPass [numChannels][numAllPasses]; //create allpass for each channel
    
    SmoothedValue<float> damping, feedback, delay;
    
    juce::AudioProcessorValueTreeState &treeParameters;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::None> delayLine{44100};
    
    float gain = 0.015f;
    double globeSampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShroederReverb)
};


