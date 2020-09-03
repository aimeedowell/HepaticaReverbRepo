//PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ReverbAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , public juce::Slider::Listener
{
public:
    ReverbAudioProcessorEditor (ReverbAudioProcessor&);
    
    //juce::AudioProcessorEditor
    ~ReverbAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //juce::Slider
    void sliderValueChanged (juce::Slider* slider) override {};

private:
    void AddCommonPluginBackground(juce::Graphics &g);
    
    void AddGainSlider();
    void AddGainSliderLabel();
    void SetGainSliderBounds();
    void SetGainSliderColour();

    ReverbAudioProcessor& audioProcessor;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Label> gainSliderLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
