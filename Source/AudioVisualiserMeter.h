//
//  AudioVisualiserMeter.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 07/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

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
    
private:
    void InitialiseAudioVisualiser();
    
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
