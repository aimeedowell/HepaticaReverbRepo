//
//  AudioVisualiserMeter.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 07/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//
#include "AudioVisualiserMeter.h"

#include "PluginEditor.h"

#include <stdio.h>

AudioVisualiserMeter::AudioVisualiserMeter(ReverbAudioProcessorEditor &editor)
: editor(editor)
, meterImage(juce::ImageCache::getFromMemory(BinaryData::VisualAudioMeter_PNG, BinaryData::VisualAudioMeter_PNGSize))
, meterWidth(80.f)
, maxMeterHeight(300.f)
, destX(0)
, destY(0)
, destWidth(meterWidth)
, sourceX(0)
, sourceWidth(meterImage.getWidth())
, externalY(0)
{
    InitialiseAudioVisualiser();
}

AudioVisualiserMeter::~AudioVisualiserMeter()
{
}

void AudioVisualiserMeter::paint(juce::Graphics &g)
{
    g.drawImage(meterImage, destX, destY, destWidth, destHeight, sourceX, sourceY, sourceWidth, sourceHeight);
}


const int AudioVisualiserMeter::GetHeight()
{
    return maxMeterHeight;
}

const int AudioVisualiserMeter::GetWidth()
{
    return meterWidth;
}

int AudioVisualiserMeter::GetYPosition()
{
    return externalY;
}

 void AudioVisualiserMeter::InitialiseAudioVisualiser()
{
    destHeight = maxMeterHeight;
    sourceY = meterImage.getHeight();
    sourceHeight = meterImage.getHeight();
    
    setSize(meterWidth, maxMeterHeight);
    UpdateAudioVisualiserHeight(0.0f);
    repaint();
}

void AudioVisualiserMeter::AddAmplitudeToBuffer(float amplitudeValue)
{
    amplitudeBuffer.push_back(amplitudeValue);

    if (amplitudeBuffer.size() > 0)
    {
        UpdateAudioVisualiserHeight(amplitudeValue);
        amplitudeBuffer.pop_back();
    }
}

 void AudioVisualiserMeter::UpdateAudioVisualiserHeight(float amplitudeValue)
{
    if (amplitudeValue < 0 || amplitudeValue > 1)
        return;
        
    sourceY = meterImage.getHeight() * (1 - amplitudeValue); // Where to start cropping original image height wise
    externalY = maxMeterHeight * (1 - amplitudeValue);       // Where to start drawing new image in editor (gap from top + this value)
    sourceHeight = meterImage.getHeight() * amplitudeValue;  // How long should the crop be?
    destHeight = maxMeterHeight * amplitudeValue;            // How long should the crop be?
    
    editor.SetAudioVisualiserBounds(this);
    repaint();
}
