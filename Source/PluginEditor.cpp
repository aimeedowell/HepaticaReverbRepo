//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (830, 350);
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ReverbAudioProcessorEditor::AddCommonPluginBackground(juce::Graphics &g)
{
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::CommonPluginBackground_PNG, BinaryData::CommonPluginBackground_PNGSize);
    
    g.drawImageWithin(background, 0, 0, getWidth() ,getHeight(), false);
}
