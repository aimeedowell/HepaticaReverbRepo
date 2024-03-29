//
//  Equalisation.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 12/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//
#include "Equalisation.h"

#include "PluginProcessor.h"
#include <stdio.h>

Equalisation::Equalisation(ReverbAudioProcessor &processor)
: audioProcessor(processor)
, noEQButton(std::make_unique<juce::ImageButton>())
, lowPassButton(std::make_unique<juce::ImageButton>())
, highPassButton(std::make_unique<juce::ImageButton>())
, lowShelfButton(std::make_unique<juce::ImageButton>())
, highShelfButton(std::make_unique<juce::ImageButton>())
, noEQ(juce::ImageCache::getFromMemory(BinaryData::X_PNG, BinaryData::X_PNGSize))
, noEQDown(juce::ImageCache::getFromMemory(BinaryData::XDown_PNG, BinaryData::XDown_PNGSize))
, lowPass(juce::ImageCache::getFromMemory(BinaryData::LP_PNG, BinaryData::LP_PNGSize))
, lowPassDown(juce::ImageCache::getFromMemory(BinaryData::LPDown_PNG, BinaryData::LPDown_PNGSize))
, highPass(juce::ImageCache::getFromMemory(BinaryData::HP_PNG, BinaryData::HP_PNGSize))
, highPassDown(juce::ImageCache::getFromMemory(BinaryData::HPDown_PNG, BinaryData::HPDown_PNGSize))
, lowShelf(juce::ImageCache::getFromMemory(BinaryData::LS_PNG, BinaryData::LS_PNGSize))
, lowShelfDown(juce::ImageCache::getFromMemory(BinaryData::LSDown_PNG, BinaryData::LSDown_PNGSize))
, highShelf(juce::ImageCache::getFromMemory(BinaryData::HS_PNG, BinaryData::HS_PNGSize))
, highShelfDown(juce::ImageCache::getFromMemory(BinaryData::HSDown_PNG, BinaryData::HSDown_PNGSize))
{
    setSize(180, 130);
    rectHeight = (2 * getHeight())/3;
    rectWidth = getWidth();
    noEQLine = getHeight()/6;
    lineThickness = 2.0f;
    
    AddEqualisationButtons();
}

Equalisation::~Equalisation()
{
    noEQButtonAttachment.reset();
    noEQButton.reset();
    lowPassButtonAttachment.reset();
    lowPassButton.reset();
    highPassButtonAttachment.reset();
    highPassButton.reset();
    lowShelfButtonAttachment.reset();
    lowShelfButton.reset();
    highShelfButtonAttachment.reset();
    highShelfButton.reset();
}

void Equalisation::paint(juce::Graphics &g)
{

    if (lowPassButton->getToggleStateValue() == true)
        DrawLowPass(g);
    else if (highPassButton->getToggleStateValue() == true)
        DrawHighPass(g);
    else if (lowShelfButton->getToggleStateValue() == true)
        DrawLowShelf(g);
    else if (highShelfButton->getToggleStateValue() == true)
        DrawHighShelf(g);
    else if (noEQButton->getToggleStateValue() == true || lowPassButton->getToggleStateValue() == false || highPassButton->getToggleStateValue() == false || lowShelfButton->getToggleStateValue() == false || highShelfButton->getToggleStateValue() == false)
        DrawNoEqualisation(g);
}

void Equalisation::resized()
{
    auto width = getWidth();
    auto height = getHeight();
    
    SetEqualisationButtonBounds(width, height);
}

void Equalisation::buttonClicked(juce::Button *button)
{
    repaint();
}

void Equalisation::buttonStateChanged(juce::Button *button)
{

}

void Equalisation::DrawLowPass(juce::Graphics &g)
{
    auto cutoff = rectWidth * 0.75;
    
    FillEQRect(g);
    
    g.setColour(juce::Colour(177, 9, 219));
    g.drawLine(0, noEQLine, cutoff, noEQLine, lineThickness * 1.5f);
    g.drawLine(cutoff - 1, noEQLine, rectWidth, rectHeight, lineThickness * 1.5f);
    
    DrawEQRect(g);
}

void Equalisation::DrawHighPass(juce::Graphics &g)
{
    auto cutoff = rectWidth * 0.25;
    
    FillEQRect(g);
    
    g.setColour(juce::Colour(177, 9, 219));
    g.drawLine(cutoff, noEQLine, rectWidth, noEQLine, lineThickness * 1.5f);
    g.drawLine(0, rectHeight, cutoff + 1, noEQLine, lineThickness * 1.5f);
    
    DrawEQRect(g);
}

void Equalisation::DrawLowShelf(juce::Graphics &g)
{
    FillEQRect(g);
    auto cutoff = rectWidth * 0.25;
    auto shelfHeight = 5;
    
    g.setColour(juce::Colour(177, 9, 219));
    g.drawLine(0, shelfHeight, cutoff - 10, shelfHeight, lineThickness * 1.5f);
    g.drawLine(cutoff - 10, shelfHeight, cutoff, noEQLine, lineThickness * 1.5f);
    g.drawLine(cutoff, noEQLine, rectWidth, noEQLine, lineThickness * 1.5f);
    
    DrawEQRect(g);
}

void Equalisation::DrawHighShelf(juce::Graphics &g)
{
    FillEQRect(g);
    auto cutoff = rectWidth * 0.75;
    auto shelfHeight = 5;
    
    g.setColour(juce::Colour(177, 9, 219));
    g.drawLine(rectWidth, shelfHeight, cutoff + 10, shelfHeight, lineThickness * 1.5f);
    g.drawLine(cutoff + 10, shelfHeight, cutoff, noEQLine, lineThickness * 1.5f);
    g.drawLine(cutoff, noEQLine, 0, noEQLine, lineThickness * 1.5f);
    
    DrawEQRect(g);
}


void Equalisation::DrawNoEqualisation(juce::Graphics &g)
{
    FillEQRect(g);
    
    g.setColour(juce::Colour(177, 9, 219));
    g.drawLine(0, noEQLine, rectWidth, noEQLine, lineThickness * 1.5f);
    
    DrawEQRect(g);
}


void Equalisation::DrawEQRect(juce::Graphics &g)
{
    auto rect = juce::Rectangle<float>(float(0), float(0), rectWidth, rectHeight);
    
    g.setOpacity(0.2f);
    g.setColour(juce::Colours::whitesmoke);
    g.drawRect(rect, lineThickness * 0.5);
}

void Equalisation::FillEQRect(juce::Graphics &g)
{
    auto rect = juce::Rectangle<float>(float(0), float(0), rectWidth, rectHeight);
    
    g.setColour(juce::Colour(177, 9, 219));
    g.setOpacity(0.2f);
    g.fillRect(rect);
}

void Equalisation::AddEqualisationButtons()
{
    juce::uint8 colour = 0;
    auto transparent = juce::Colour(colour, colour, colour, 0.0f);
    
    addAndMakeVisible(noEQButton.get());
    addAndMakeVisible(lowPassButton.get());
    addAndMakeVisible(highPassButton.get());
    addAndMakeVisible(lowShelfButton.get());
    addAndMakeVisible(highShelfButton.get());
    
    noEQButton->addListener(this);
    lowPassButton->addListener(this);
    highPassButton->addListener(this);
    lowShelfButton->addListener(this);
    highShelfButton->addListener(this);
    
    noEQButton.get()->setImages(false, true, true, noEQ, 0.7f, transparent, noEQDown, 0.5f, transparent, noEQDown, 1.0f,
        transparent);
    lowPassButton.get()->setImages(false, true, true, lowPass, 0.7f, transparent, lowPassDown, 0.5f, transparent, lowPassDown, 1.0f, transparent);
    highPassButton.get()->setImages(false, true, true, highPass, 0.7f, transparent, highPassDown, 0.5f, transparent, highPassDown, 1.0f, transparent);
    lowShelfButton.get()->setImages(false, true, true, lowShelf, 0.7f, transparent, lowShelfDown, 0.5f, transparent, lowShelfDown, 1.0f, transparent);
    highShelfButton.get()->setImages(false, true, true, highShelf, 0.7f, transparent, highShelfDown, 0.5f, transparent, highShelfDown, 1.0f, transparent);
    
    noEQButton->setClickingTogglesState(true);
    lowPassButton->setClickingTogglesState(true);
    highPassButton->setClickingTogglesState(true);
    lowShelfButton->setClickingTogglesState(true);
    highShelfButton->setClickingTogglesState(true);
    
    noEQButton->setRadioGroupId(34567);
    lowPassButton->setRadioGroupId(34567);
    highPassButton->setRadioGroupId(34567);
    lowShelfButton->setRadioGroupId(34567);
    highShelfButton->setRadioGroupId(34567);
    
    noEQButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "noEQID"
                                                                                                , *noEQButton.get());
    lowPassButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "lowPassID"
                                                                                                , *lowPassButton.get());
    highPassButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "highPassID"
                                                                                                , *highPassButton.get());
    lowShelfButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "lowShelfID"
                                                                                                , *lowShelfButton.get());
    highShelfButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "highShelfID"
                                                                                                , *highShelfButton.get());
}

void Equalisation::SetEqualisationButtonBounds(int width, int height)
{
    noEQButton->setBounds(width/40, (2*height)/3, width/6.7, height/5);
    lowPassButton->setBounds(width/4.4, (2*height)/3, width/6.7, height/5);
    highPassButton->setBounds(width/2.35, (2*height)/3, width/6.7, height/5);
    lowShelfButton->setBounds(width/1.6, (2*height)/3, width/6.7, height/5);
    highShelfButton->setBounds(width/1.21, (2*height)/3, width/6.7, height/5);
}
