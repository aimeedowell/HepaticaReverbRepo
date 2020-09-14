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
, textForLabel("DEFAULT")
{
    setSize(270, 25);
    AddPresetLabel();
}


PresetBar::~PresetBar()
{
    presetLabel.reset();
}

void PresetBar::paint(juce::Graphics &g)
{
    DrawPresetBarLines(g);
}

void PresetBar::resized()
{
    SetLabelBounds();
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
