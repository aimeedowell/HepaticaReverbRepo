//
//  AudioVisualiserMeter.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 07/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

/*
  This class creates an audio visualiser meter which uses the peak amplitude from each processing block. A full image is created (by the author in procreate), the height cropped based on the amplitude value and then the component is repainted.
 
 A reference to the editor is created to set the bounds of the component. The editor will control access to the Plugin Processor and to control the timer used to set the update frequency for the meter.
 
 */

#ifndef AudioVisualiserMeter_h
#define AudioVisualiserMeter_h

#include <JuceHeader.h>

class ReverbAudioProcessorEditor;

class AudioVisualiserMeter
    : public juce::Component
{
public:
    AudioVisualiserMeter(ReverbAudioProcessorEditor &editor);
    ~AudioVisualiserMeter();
    
    //juce::Component
    void paint(juce::Graphics&) override;
    void resized() override {};
    
    const int GetHeight();
    
    const int GetWidth();
    
    int GetYPosition();
    
    void AddAmplitudeToBuffer(float amplitudeValue);
    
private:
    void InitialiseAudioVisualiser();
    
    void UpdateAudioVisualiserHeight(float amplitudeValue);
    
    ReverbAudioProcessorEditor &editor;
    juce::Image meterImage;
    std::vector<float> amplitudeBuffer;

    const float meterWidth;
    const float maxMeterHeight;
    
    const int destX;
    const int destY;
    const int destWidth;
    const int sourceX;
    const int sourceWidth;
    int destHeight;
    int sourceY;
    int sourceHeight;
    int externalY;
};
#endif /* AudioVisualiserMeter_h */
