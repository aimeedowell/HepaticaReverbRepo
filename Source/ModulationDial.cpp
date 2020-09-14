//
//  ModulationDial.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 14/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//
#include "ModulationDial.h"

#include <stdio.h>

ModulationDial::ModulationDial()
: sliderImage(juce::ImageCache::getFromMemory(BinaryData::ModulationSlider_PNG, BinaryData::ModulationSlider_PNGSize))
, sliderComponent(std::make_unique<juce::ImageComponent>())
{
    setSize(75, 75);
    AddSliderImage();
ModulationDial::~ModulationDial()
{
    sliderComponent.reset();
}
void ModulationDial::resized()
{
    const int width = GetModDialWidth();
    const int height = GetModDialWidth();
    SetSliderImageBounds(width, height);
}

void ModulationDial::AddSliderImage()
{
    addAndMakeVisible(sliderComponent.get());
    sliderComponent.get()->setImage(sliderImage, juce::RectanglePlacement::stretchToFit);
}

void ModulationDial::SetSliderImageBounds(int width, int height)
{
    sliderComponent.get()->setBounds(0 , 0, width, height);
}
}
