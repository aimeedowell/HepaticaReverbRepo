//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , gainSlider(std::make_unique<juce::Slider>())
{
    setSize (830, 350);
    
    AddGainSlider();
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
}

void ReverbAudioProcessorEditor::paint (juce::Graphics &g)
{
    AddCommonPluginBackground(g);
}

void ReverbAudioProcessorEditor::resized()
{
    SetGainSliderBounds();
}

void ReverbAudioProcessorEditor::AddCommonPluginBackground(juce::Graphics &g)
{
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::CommonPluginBackground_PNG, BinaryData::CommonPluginBackground_PNGSize);
    
    g.drawImageWithin(background, 0, 0, getWidth() ,getHeight(), false);
}

void ReverbAudioProcessorEditor::AddGainSlider()
{
    addAndMakeVisible(gainSlider.get());
    gainSlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider->setRange(-12.0, 6.0);
    gainSlider->setValue(0.0);
    gainSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    gainSlider->setDoubleClickReturnValue(true, 0.0);
    gainSlider->addListener(this);
    
    SetGainSliderColour();
}

void ReverbAudioProcessorEditor::SetGainSliderBounds()
{
    auto width = getWidth();
    auto height = getHeight();
    gainSlider->setBounds(width - 150, 20 , 60, 300);
}
