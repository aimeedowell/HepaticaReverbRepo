//
//  CustomLookAndFeel.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 08/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

#ifndef CustomLookAndFeel_h
#define CustomLookAndFeel_h

#include <JuceHeader.h>

class CustomLookAndFeel
    : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();
    
    juce::Font getLabelFont(juce::Label&) override;
    
    void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle, juce::Slider&) override;
    
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                             float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                             juce::Slider&) override;
    
    void SetDefaultSliderColour(juce::Slider &slider);
    
    void DrawDottedSlider(juce::Graphics &g, int x, int y, int trackWidth, int width, int height, float sliderPos);
    
    void SetDottedSliderColours(juce::Slider &slider);
    
    void SetDefaultRotarySliderColour(juce::Slider &slider);
    
    void DrawFlowerSlider(juce::Graphics &g, juce::Slider &slider, float toAngle, float centreX, float centreY, int width, int height);
    
    void SetModulationSliderColour(juce::Slider &slider);
    
private:
    juce::Image flowerImage;
    juce::Image dotSliderImage;
};
#endif /* CustomLookAndFeel_h */

