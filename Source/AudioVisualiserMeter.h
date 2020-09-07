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
private:
    ReverbAudioProcessorEditor &editor;
};
#endif /* AudioVisualiserMeter_h */
