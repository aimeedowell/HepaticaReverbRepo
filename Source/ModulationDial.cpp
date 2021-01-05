//
//  ModulationDial.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 14/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//
#include "ModulationDial.h"

#include "PluginProcessor.h"

#include <stdio.h>

ModulationDial::ModulationDial(ReverbAudioProcessor& p, juce::String treeID)
: audioProcessor(p)
, sliderImage(juce::ImageCache::getFromMemory(BinaryData::ModulationSlider_PNG, BinaryData::ModulationSlider_PNGSize))
, sliderComponent(std::make_unique<juce::ImageComponent>())
, modSlider(std::make_unique<juce::Slider>())
, treeID(treeID)
{
    setSize(75, 75);
    AddSliderImage();
    AddRotarySlider();
}

ModulationDial::~ModulationDial()
{
    sliderComponent.reset();
    modSliderAttachment.reset();
    modSlider.reset();
}

void ModulationDial::paint(juce::Graphics &g)
{
    
}

void ModulationDial::resized()
{
    const int width = GetModDialWidth();
    const int height = GetModDialWidth();
    SetSliderImageBounds(width, height);
    SetRotarySliderBounds(width, height);
}

void ModulationDial::AddSliderImage()
{
    addAndMakeVisible(sliderComponent.get());
    sliderComponent.get()->setImage(sliderImage, juce::RectanglePlacement::stretchToFit);
}

void ModulationDial::AddRotarySlider()
{
    addAndMakeVisible(modSlider.get());
    modSlider.get()->addListener(this);
    modSlider.get()->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , treeID
                                                                                                   , *modSlider.get());
    
}

void ModulationDial::SetSliderImageBounds(int width, int height)
{
    sliderComponent.get()->setBounds(0 , 0, width, height);
}

void ModulationDial::SetRotarySliderBounds(int width, int height)
{
    auto sliderSize = width - 1;
    modSlider.get()->setBounds(1, 0, sliderSize, height);
}
