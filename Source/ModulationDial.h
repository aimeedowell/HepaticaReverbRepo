//
//  ModulationDial.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 14/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

#ifndef ModulationDial_h
#define ModulationDial_h

#include <JuceHeader.h>

class ReverbAudioProcessor;

class ModulationDial
    : public juce::Slider
    , public juce::Slider::Listener
{
public:
    ModulationDial(ReverbAudioProcessor& p, juce::String treeID);
    ~ModulationDial();
    
    //juce::Slider
    void sliderValueChanged(juce::Slider* slider) override {};
    
    const int GetModDialHeight() { return getHeight(); };
    const int GetModDialWidth() { return getWidth(); };
    
private:
    //juce::Component
    void paint(juce::Graphics&) override;
    void resized() override;
    
    void AddSliderImage();
    void SetSliderImageBounds(int width, int height);
    
    void AddRotarySlider();
    void SetRotarySliderBounds(int width, int height);
    
    ReverbAudioProcessor &audioProcessor;
    
    juce::Image sliderImage;
    std::unique_ptr<juce::ImageComponent> sliderComponent;
    
    std::unique_ptr<juce::Slider> modSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modSliderAttachment;
    
    juce::String treeID;
};

#endif /* ModulationDial_h */
