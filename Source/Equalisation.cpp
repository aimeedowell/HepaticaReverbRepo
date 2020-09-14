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

void Equalisation::resized()
{
    auto width = getWidth();
    auto height = getHeight();
    
    SetEqualisationButtonBounds(width, height);
}

void Equalisation::buttonClicked(juce::Button *button)
{
    if (button == noEQButton.get())
    {
        lowPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
    else if (button == lowPassButton.get())
    {
        noEQButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
    else if (button == highPassButton.get())
    {
        noEQButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
    else if (button == lowShelfButton.get())
    {
        noEQButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
    else if (button == highShelfButton.get())
    {
        noEQButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        lowShelfButton->setToggleState(false, juce::NotificationType::dontSendNotification);
        highPassButton->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
void Equalisation::buttonStateChanged(juce::Button *button)
{

}
void Equalisation::DrawEQRect(juce::Graphics &g)
{
    auto rect = juce::Rectangle<float>(float(0), float(0), rectWidth, rectHeight);
    
    g.setColour(juce::Colours::grey);
    g.drawRect(rect, lineThickness);
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
    
    noEQButton.get()->addListener(this);
    lowPassButton.get()->addListener(this);
    highPassButton.get()->addListener(this);
    lowShelfButton.get()->addListener(this);
    highShelfButton.get()->addListener(this);
    
    noEQButton.get()->setImages(false, true, true, noEQ, 0.7f, transparent, noEQDown, 0.5f, transparent, noEQDown, 1.0f,
        transparent);
    lowPassButton.get()->setImages(false, true, true, lowPass, 0.7f, transparent, lowPassDown, 0.5f, transparent, lowPassDown, 1.0f, transparent);
    highPassButton.get()->setImages(false, true, true, highPass, 0.7f, transparent, highPassDown, 0.5f, transparent, highPassDown, 1.0f, transparent);
    lowShelfButton.get()->setImages(false, true, true, lowShelf, 0.7f, transparent, lowShelfDown, 0.5f, transparent, lowShelfDown, 1.0f, transparent);
    highShelfButton.get()->setImages(false, true, true, highShelf, 0.7f, transparent, highShelfDown, 0.5f, transparent, highShelfDown, 1.0f, transparent);
    
    noEQButton.get()->setClickingTogglesState(true);
    lowPassButton.get()->setClickingTogglesState(true);
    highPassButton.get()->setClickingTogglesState(true);
    lowShelfButton.get()->setClickingTogglesState(true);
    highShelfButton.get()->setClickingTogglesState(true);
    
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
