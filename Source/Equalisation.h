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
    
    void AddEqualisationButtons();
    
    void SetEqualisationButtonBounds(int width, int height);
    ReverbAudioProcessor &audioProcessor;
    
    std::unique_ptr<juce::ImageButton> noEQButton;
    std::unique_ptr<juce::ImageButton> lowPassButton;
    std::unique_ptr<juce::ImageButton> highPassButton;
    std::unique_ptr<juce::ImageButton> lowShelfButton;
    std::unique_ptr<juce::ImageButton> highShelfButton;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> noEQButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowPassButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highPassButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowShelfButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highShelfButtonAttachment;
    
    juce::Image noEQ;
    juce::Image noEQDown;
    juce::Image lowPass;
    juce::Image lowPassDown;
    juce::Image highPass;
    juce::Image highPassDown;
    juce::Image lowShelf;
    juce::Image lowShelfDown;
    juce::Image highShelf;
    juce::Image highShelfDown;
};

#endif /* Equalisation_h */
