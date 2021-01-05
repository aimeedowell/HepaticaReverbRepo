//PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "CustomLookAndFeel.h"

class AudioVisualiserMeter;
class Equalisation;
class ModulationDial;
class PresetBar;

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
    
    void AddPresetBar();

    void AddEqualisationGraph();

    ReverbAudioProcessor &audioProcessor;
    CustomLookAndFeel reverbLookAndFeel;
    
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
    
    std::unique_ptr<PresetBar> presetBar;
    
    std::unique_ptr<Equalisation> eqGraph;
    std::unique_ptr<juce::Label> eqGraphLabel;
    
    juce::uint8 labelColour = 245;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbAudioProcessorEditor)
};
