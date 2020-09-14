//
//  Equalisation.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 12/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

#ifndef Equalisation_h
#define Equalisation_h

#include <JuceHeader.h>

class ReverbAudioProcessor;

class Equalisation
    : public juce::Component
    , public juce::Button::Listener
{
public:
    Equalisation(ReverbAudioProcessor &processor);
    ~Equalisation();
private:
    //juce::Component
    void paint(juce::Graphics&) override;
    void resized() override;
    
    //juce::ButtonListener
    void buttonClicked(juce::Button*) override;
    void buttonStateChanged(juce::Button*) override;
    ReverbAudioProcessor &audioProcessor;
};

#endif /* Equalisation_h */
