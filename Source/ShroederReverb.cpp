/*
  ==============================================================================

    ShroederReverb.cpp
    Created: 5 Jan 2021 10:41:37am
    Author:  Aimee Dowell

  ==============================================================================
*/

#include "ShroederReverb.h"

ShroederReverb::ShroederReverb(juce::AudioProcessorValueTreeState &valueTreeState)
: mixer(std::make_unique<juce::dsp::DryWetMixer<float>>())
, treeParameters(valueTreeState)
{
    prepareToPlay(44100.0, 100);
    UpdateDamping();
    mixer->setMixingRule(juce::dsp::DryWetMixingRule::linear);
}

ShroederReverb::~ShroederReverb()
{
    mixer.reset();
}

void ShroederReverb::prepareToPlay(const double sampleRate, int samplesPerBlock)
{
    jassert (sampleRate > 0);
    globeSampleRate = sampleRate;
    
    const int stereoSpread = 23;

    TuneFilters((int)stereoSpread, sampleRate);

    const double smoothTime = 0.01;
    damping .reset (sampleRate, smoothTime);
    feedback.reset (sampleRate, smoothTime);
    
    SetCoefficients();
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    delayLine.reset();
    delayLine.prepare(spec);
    processorChain.reset();
    processorChain.prepare(spec);
    mixer->prepare(spec);
    
    float preDelay = *treeParameters.getRawParameterValue("preDelayID") * 1000;
    delay.setTargetValue(preDelay);
    
    delayLine.setDelay(delay.getNextValue());
}

void ShroederReverb::reset()
{
    for (int j = 0; j < numChannels; ++j)
    {
        for (int i = 0; i < numCombs; ++i)
            comb[j][i].clear();

        for (int i = 0; i < numAllPasses; ++i)
            allPass[j][i].clear();
    }
}

void ShroederReverb::process(juce::AudioBuffer<float> &buffer) noexcept
{
    auto wetSignal = buffer;
    
    auto *wetSignalL = wetSignal.getWritePointer(0);
    auto *wetSignalR = wetSignal.getWritePointer(1);
    
    juce::dsp::AudioBlock<float> dryBlock(buffer);
    
    float dryWetLevel = *treeParameters.getRawParameterValue("wetDryID")/100;
    
    mixer->setWetMixProportion(dryWetLevel);
    mixer->pushDrySamples(dryBlock);

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        const float input = (wetSignalL[i] + wetSignalR[i]) * gain;
        float wetL = 0, wetR = 0;
        
        UpdateDamping();
        
        const float damp    = damping.getNextValue();
        const float feedbck = feedback.getNextValue();
        
        float earlyGain = *treeParameters.getRawParameterValue("earlyReflectionsID")/100;
        earlyReflectFeedback.setTargetValue(earlyGain);
        
        for (int j = 0; j < numAllPasses; ++j)  // run the early reflection allpass filters in series
        {
            wetL = earlyReflections[0][j].process(wetL, earlyReflectFeedback.getNextValue());
            wetR = earlyReflections[1][j].process(wetR, earlyReflectFeedback.getNextValue());
        }

        for (int j = 0; j < numCombs; ++j)  // accumulate the comb filters in parallel
        {
            wetL += comb[0][j].process(input, damp, feedbck);
            wetR += comb[1][j].process(input, damp, feedbck);
        }

        for (int j = 0; j < numAllPasses; ++j)  // run the allpass filters in series
        {
            wetL = allPass[0][j].process(wetL, 0.5f);
            wetR = allPass[1][j].process(wetR, 0.5f);
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
        
        // Apply stereo widdening to the signals
        float stereoWidth = *treeParameters.getRawParameterValue("stereoSpreadID")/100;
        auto side = 0.5 * (wetL - wetR);
        auto mid = 0.5 * (wetL + wetR);
        auto scaledSide = stereoWidth * side;
        auto scaledMid = (2 - stereoWidth) * mid;

        wetL = scaledMid + scaledSide;
        wetR = scaledMid - scaledSide;
        
        wetSignalL[i] = wetL;
        wetSignalR[i] = wetR;
    }
    juce::dsp::AudioBlock<float> wetBlock(wetSignal);
    juce::dsp::ProcessContextReplacing<float> context(wetBlock);
    
    UpdateFilters();
    
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(wetBlock));
    
    mixer->mixWetSamples(context.getOutputBlock());

}

void ShroederReverb::UpdateDamping() noexcept
{
    const float roomScaleFactor = 0.28f;
    const float roomOffset = 0.7f;
    const float dampScaleFactor = 0.4f;
    
    float roomSizeValue = *treeParameters.getRawParameterValue("reverbSizeID")/100;
    float dampValue = *treeParameters.getRawParameterValue("dampingID") /100;
    damping.setTargetValue(dampValue);

    SetDamping(damping.getNextValue() * dampScaleFactor,
                    roomSizeValue * roomScaleFactor + roomOffset);
}

void ShroederReverb::SetDamping (const float dampingToUse, const float roomSizeToUse) noexcept
{
    damping.setTargetValue (dampingToUse);
    feedback.setTargetValue (roomSizeToUse);
}

void ShroederReverb::TuneFilters(int stereoSpread, double sampleRate)
{
    for (int i = 0; i < numAllPasses; ++i)
    {
        earlyReflections[0][i].setSize (((int)sampleRate * (earlyReflectTunings[i])) / 44100);
        earlyReflections[1][i].setSize (((int)sampleRate * (earlyReflectTunings[i] + stereoSpread)) / 44100);
    }
    
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

void ShroederReverb::SetCoefficients()
{
    auto &filter1 = processorChain.get<0>();
    auto &filter2 = processorChain.get<1>();
    auto &filter3 = processorChain.get<2>();
    auto &filter4 = processorChain.get<3>();
    
    *filter1.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(globeSampleRate, 500, 3);
    *filter2.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(globeSampleRate, 3000, 3);
    *filter3.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(globeSampleRate, 500, 3, 3.f);
    *filter4.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(globeSampleRate, 3000, 3, 3.f);
}

void ShroederReverb::UpdateFilters()
{
    bool isLowPass = *treeParameters.getRawParameterValue("lowPassID");
    bool isHighPass = *treeParameters.getRawParameterValue("highPassID");
    bool isLowShelf = *treeParameters.getRawParameterValue("lowShelfID");
    bool isHighShelf = *treeParameters.getRawParameterValue("highShelfID");
    
    if (isLowPass)
    {
        processorChain.setBypassed<0>(false);
        processorChain.setBypassed<1>(true);
        processorChain.setBypassed<2>(true);
        processorChain.setBypassed<3>(true);
    }
    else if (isHighPass)
    {
        processorChain.setBypassed<0>(true);
        processorChain.setBypassed<1>(false);
        processorChain.setBypassed<2>(true);
        processorChain.setBypassed<3>(true);
    }
    else if (isLowShelf)
    {
        processorChain.setBypassed<0>(true);
        processorChain.setBypassed<1>(true);
        processorChain.setBypassed<2>(false);
        processorChain.setBypassed<3>(true);
    }
    else if (isHighShelf)
    {
        processorChain.setBypassed<0>(true);
        processorChain.setBypassed<1>(true);
        processorChain.setBypassed<2>(true);
        processorChain.setBypassed<3>(false);
    }
    else
    {
        processorChain.setBypassed<0>(true);
        processorChain.setBypassed<1>(true);
        processorChain.setBypassed<2>(true);
        processorChain.setBypassed<3>(true);
    }
}
