//PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ReverbAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    ReverbAudioProcessorEditor (ReverbAudioProcessor&);
    
    //juce::AudioProcessorEditor
    ~ReverbAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void AddCommonPluginBackground(juce::Graphics &g);

    ReverbAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
