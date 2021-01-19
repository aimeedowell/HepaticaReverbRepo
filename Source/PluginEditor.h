//PluginEditor.h

/*
 This class controls the graphical interface of the plugin.
 
 A custom look and feel class is initialised here to override any default methods for the sliders, the equalisation class is also initalised here and the modulation dials are initalised for the rotary sliders which want a different style.
 
 The audio visualiser meter is also initialised here and is updated using a juce::Timer every 10ms. As the editor takes a reference to the PluginProcessor, values from the small amplitude buffer can be sent to each audio visualiser object. There are two visualiser objects, one for the left channel, and one for the right channel.

 Image buttons have been created using PNG images, drawn by the author in Procreate. In cases where only one button should be selected at a time, it is assigned to a radio button group ID.
 
 */

#pragma once

#include <JuceHeader.h>

#include "CustomLookAndFeel.h"

class AudioVisualiserMeter;
class Equalisation;
class ModulationDial;
class ReverbAudioProcessor;

class ReverbAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , public juce::Slider::Listener
    , public juce::Button::Listener
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
    
    //juce::ButtonListener
    void buttonClicked(juce::Button*) override {};
    void buttonStateChanged(juce::Button*) override {};
    
    //juce::Timer
    void timerCallback() override;
    
    void SetAudioVisualiserBounds(AudioVisualiserMeter *meter); //This is a public method as it gets updated by AudioVisualiserMeter

private:
    void AddCommonPluginBackground(juce::Graphics &g);
    void DrawTitle(juce::Graphics &g);
    
    void AddBypassButton();
    void SetBypassButtonBounds(int width, int height);
    
    void AddGainSlider();
    void SetGainSliderBounds(int width, int height);
    
    void AddPanSlider();
    void SetPanSliderBounds(int width, int height);
    
    void AddWetDrySlider();
    void SetWetDrySliderBounds(int width, int height);
    
    void AddReverbSizeSlider();
    void SetReverbSizeSliderBounds(int width, int height);
    
    void AddPreDelaySlider();
    void SetPreDelaySliderBounds(int width, int height);
    
    void AddEarlyReflectionsSlider();
    void SetEarlyReflectionsSliderBounds(int width, int height);
    
    void AddModulationSliders();
    void SetModulationSliderBounds(int width, int height);
    
    void AddAudioVisualiser();

    void AddEqualisationGraph();

    ReverbAudioProcessor &audioProcessor;
    CustomLookAndFeel reverbLookAndFeel;
    
    std::unique_ptr<juce::ImageButton> bypassButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassButtonAttachment;
    
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Label> gainSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    
    std::unique_ptr<juce::Slider> panSlider;
    std::unique_ptr<juce::Label> leftPanSliderLabel;
    std::unique_ptr<juce::Label> rightPanSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panSliderAttachment;
    
    std::unique_ptr<juce::Slider> wetDrySlider;
    std::unique_ptr<juce::Label> drySliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetDrySliderAttachment;
    
    std::unique_ptr<juce::Slider> reverbSizeSlider;
    std::unique_ptr<juce::Label> reverbSizeSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbSizeSliderAttachment;
    
    std::unique_ptr<juce::Slider> preDelaySlider;
    std::unique_ptr<juce::Label> preDelaySliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preDelaySliderAttachment;
    
    std::unique_ptr<juce::Slider> earlyReflectionsSlider;
    std::unique_ptr<juce::Label> earlyReflectionsSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> earlyReflectionsSliderAttachment;
    
    std::unique_ptr<ModulationDial> stereoSpread;
    std::unique_ptr<ModulationDial> damping;
    std::unique_ptr<juce::Label> stereoSpreadLabel;
    std::unique_ptr<juce::Label> dampingLabel;
    
    std::unique_ptr<AudioVisualiserMeter> leftAudioMeter;
    std::unique_ptr<AudioVisualiserMeter> rightAudioMeter;
    
    std::unique_ptr<Equalisation> eqGraph;
    std::unique_ptr<juce::Label> eqGraphLabel;
    
    juce::Image title = juce::ImageCache::getFromMemory(BinaryData::Title_PNG, BinaryData::Title_PNGSize);
    
    juce::uint8 labelColour = 245;
    juce::Colour noColour = juce::Colour(labelColour, labelColour, labelColour, 0.0f);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
