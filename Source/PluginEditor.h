//PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class PitchShiftAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    PitchShiftAudioProcessorEditor (PitchShiftAudioProcessor&);
    
    //juce::AudioProcessorEditor
    ~PitchShiftAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void addCommonPluginBackground(juce::Graphics &g);

    PitchShiftAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchShiftAudioProcessorEditor)
};
