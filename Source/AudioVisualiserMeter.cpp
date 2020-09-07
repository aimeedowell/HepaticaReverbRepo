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
, meterImage(juce::ImageCache::getFromMemory(BinaryData::VisualAudioMeter_PNG, BinaryData::CommonPluginBackground_PNGSize))
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
    repaint();
}
