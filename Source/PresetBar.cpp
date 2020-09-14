//
//  PresetBar.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 14/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

#include "PresetBar.h"

#include <stdio.h>

PresetBar::PresetBar()
: presetLabel(std::make_unique<juce::Label>())
, nextPreset(std::make_unique<juce::ImageButton>())
, previousPreset(std::make_unique<juce::ImageButton>())
, nextPresetImage(juce::ImageCache::getFromMemory(BinaryData::NextPreset_PNG, BinaryData::NextPreset_PNGSize))
, nextPresetImageDown(juce::ImageCache::getFromMemory(BinaryData::HepaticaNextDown_PNG, BinaryData::HepaticaNextDown_PNGSize))
, prevPresetImage(juce::ImageCache::getFromMemory(BinaryData::PrevPreset_PNG, BinaryData::PrevPreset_PNGSize))
, prevPresetImageDown(juce::ImageCache::getFromMemory(BinaryData::HepaticaPrevDown_PNG, BinaryData::HepaticaPrevDown_PNGSize))
, textForLabel("DEFAULT")
{
    setSize(270, 25);
    AddPresetLabel();
    SetPresetButtons();
}


PresetBar::~PresetBar()
{
    presetLabel.reset();
    nextPreset.reset();
    previousPreset.reset();
}

void PresetBar::paint(juce::Graphics &g)
{
    DrawPresetBarLines(g);
}

void PresetBar::resized()
{
    SetLabelBounds();
    SetPresetButtonBounds();
}

juce::String PresetBar::SetPresetText(juce::String preset)
{
    return textForLabel = preset;
}

juce::String PresetBar::GetPresetText()
{
    return textForLabel;
}

void PresetBar::AddPresetLabel()
{
    addAndMakeVisible(presetLabel.get());
    presetLabel.get()->setText(textForLabel, juce::NotificationType::dontSendNotification);
    presetLabel.get()->setJustificationType(juce::Justification::horizontallyCentred);
    presetLabel.get()->setColour(juce::Label::textColourId, juce::Colour(colour, colour, colour, 0.6f));
}

void PresetBar::SetLabelBounds()
{
    presetLabel.get()->setBounds(0, 0, getWidth(), getHeight());
}

void PresetBar::DrawPresetBarLines(juce::Graphics &g)
{
    auto lineThickness = 2.5f;
    g.setColour(juce::Colour(colour, colour, colour));
    
    g.drawLine(5 + presetButtonWidth, 0, barWidth + 5 + presetButtonWidth, 0, lineThickness);
    g.drawLine(5 + presetButtonWidth, getHeight(), barWidth + 5 + presetButtonWidth, getHeight(), lineThickness);
}

void PresetBar::SetPresetButtons()
{
    auto transparent = juce::Colour(colour, colour, colour, 0.0f);
    
    addAndMakeVisible(nextPreset.get());
    addAndMakeVisible(previousPreset.get());
    
    nextPreset.get()->addListener(this);
    previousPreset.get()->addListener(this);
    
    nextPreset.get()->setImages(false, true, true, nextPresetImage, 1.0f, transparent, nextPresetImageDown, 0.6f, transparent, nextPresetImageDown, 1.0f,
    transparent);
    previousPreset.get()->setImages(false, true, true, prevPresetImage, 1.0f, transparent, prevPresetImageDown, 0.6f, transparent, prevPresetImageDown, 1.0f,
    transparent);
}

void PresetBar::SetPresetButtonBounds()
{
    previousPreset.get()->setBounds(0, 4, presetButtonWidth, getHeight() * 0.75);
    nextPreset.get()->setBounds(getWidth() - presetButtonWidth, 4, presetButtonWidth, getHeight() * 0.75);
}

