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

class ModulationDial
    : public juce::Slider
    , public juce::Slider::Listener
{
public:
    ModulationDial();
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
    
    juce::Image sliderImage;
    std::unique_ptr<juce::ImageComponent> sliderComponent;
    
};

#endif /* ModulationDial_h */
