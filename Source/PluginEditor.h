//PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "CustomLookAndFeel.h"

class AudioVisualiserMeter;

class ReverbAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , public juce::Slider::Listener
    , private juce::Timer
{
public:
    ReverbAudioProcessorEditor(ReverbAudioProcessor&);
    
    //juce::AudioProcessorEditor
    ~ReverbAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    
    //juce::Slider
    void sliderValueChanged(juce::Slider* slider) override {};
    
    //juce::Timer
    void timerCallback() override;
    
    void SetAudioVisualiserBounds(AudioVisualiserMeter *meter); //This is a public method as it gets updated by AudioVisualiserMeter

private:
    void AddCommonPluginBackground(juce::Graphics &g);
    
    void AddGainSlider();
    void AddGainSliderLabel();
    void SetGainSliderBounds(int width, int height);
    
    void AddReverbSizeSlider();
    void SetReverbSizeSliderBounds(int width, int height);
    
    void AddPreDelaySlider();
    void SetPreDelaySliderBounds(int width, int height);
    
    void AddEarlyReflectionsSlider();
    void SetEarlyReflectionsSliderBounds(int width, int height);
    
    void AddAudioVisualiser();

    ReverbAudioProcessor& audioProcessor;
    CustomLookAndFeel reverbLookAndFeel;
    
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Label> gainSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    
    std::unique_ptr<juce::Slider> reverbSizeSlider;
    std::unique_ptr<juce::Label> reverbSizeSliderLabel;
    
    std::unique_ptr<juce::Slider> preDelaySlider;
    std::unique_ptr<juce::Label> preDelaySliderLabel;
    
    std::unique_ptr<juce::Slider> earlyReflectionsSlider;
    std::unique_ptr<juce::Label> earlyReflectionsSliderLabel;
    
    std::unique_ptr<AudioVisualiserMeter> leftAudioMeter;
    std::unique_ptr<AudioVisualiserMeter> rightAudioMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
