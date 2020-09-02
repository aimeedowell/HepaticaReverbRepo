//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

PitchShiftAudioProcessorEditor::PitchShiftAudioProcessorEditor (PitchShiftAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (830, 350);
}

PitchShiftAudioProcessorEditor::~PitchShiftAudioProcessorEditor()
{
}

void PitchShiftAudioProcessorEditor::paint (juce::Graphics &g)
{
    addCommonPluginBackground(g);
}

void PitchShiftAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void PitchShiftAudioProcessorEditor::addCommonPluginBackground(juce::Graphics &g)
{
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::CommonPluginBackground_PNG, BinaryData::CommonPluginBackground_PNGSize);
    
    g.drawImageWithin(background, 0, 0, getWidth() ,getHeight(), false);
}
