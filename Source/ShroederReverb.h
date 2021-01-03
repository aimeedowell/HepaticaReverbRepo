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
        prepareToPlay(44100.0);
        updateDamping();
    }

    //==============================================================================
    /** Holds the parameters being used by a Reverb object. */
    struct Parameters
    {
        float roomSize   = 0.5f;     /**< Room size, 0 to 1.0, where 1.0 is big, 0 is small. */
        float damping    = 0.5f;     /**< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped. */
        float width      = 1.0f;     /**< Reverb width, 0 to 1.0, where 1.0 is very wide. */
    };

    void prepareToPlay(const double sampleRate)
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
            float outL = 0, outR = 0;
            
            updateDamping();
            
            const float damp    = damping.getNextValue();
            const float feedbck = feedback.getNextValue();

            for (int j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
            {
                outL += comb[0][j].process (input, damp, feedbck);
                outR += comb[1][j].process (input, damp, feedbck);
            }

            for (int j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
            {
                outL = allPass[0][j].process (outL);
                outR = allPass[1][j].process (outR);
            }
            
            float dryWetLevel = *treeParameters.getRawParameterValue("wetDryID")/100;

            left[i]  = (outL * dryWetLevel) + (left[i]  * (1 - dryWetLevel));
            right[i] = (outR * dryWetLevel) + (right[i] * (1 - dryWetLevel));
        }
    }

private:

    void updateDamping() noexcept
    {
        const float roomScaleFactor = 0.28f;
        const float roomOffset = 0.7f;
        const float dampScaleFactor = 0.4f;
        
        float roomSizeValue = *treeParameters.getRawParameterValue("reverbSizeID")/100;

        setDamping(parameters.damping * dampScaleFactor,
                        roomSizeValue * roomScaleFactor + roomOffset);
    }

    void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept
    {
        damping.setTargetValue (dampingToUse);
        feedback.setTargetValue (roomSizeToUse);
    }

    static const int numCombs = 4, numAllPasses = 2, numChannels = 2;

    Parameters parameters;
    float gain = 0.015f;;

    CombFilter comb [numChannels][numCombs]; //create comb for each channel
    AllPassFilter allPass [numChannels][numAllPasses]; //create allpass for each channel

    SmoothedValue<float> damping, feedback, dryGain, wetGain1, wetGain2;
    
    juce::AudioProcessorValueTreeState &treeParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShroederReverb)
};


