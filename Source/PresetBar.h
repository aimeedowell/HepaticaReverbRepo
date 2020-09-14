//
//  PresetBar.h
//  Hepatica Reverb
//
//  Created by Aimee Dowell on 14/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//

#ifndef PresetBar_h
#define PresetBar_h

#include <JuceHeader.h>

class PresetBar
    : public juce::Component
    , public juce::Button::Listener
{
public:
    PresetBar();
    ~PresetBar();
    
    //juce::Component
    void paint(juce::Graphics&) override;
    void resized() override;
    
    //juce::ButtonListener
    void buttonClicked(juce::Button*) override {};
    void buttonStateChanged(juce::Button*) override {};
    
    const int GetPresetBarWidth() { return getWidth(); };
    const int GetPresetBarHeight() { return getHeight(); };
    
    juce::String SetPresetText(juce::String preset);
    juce::String GetPresetText();
    
private:
    
    void AddPresetLabel();
    void SetLabelBounds();
    
    std::unique_ptr<juce::Label> presetLabel;
    juce::String textForLabel;
    juce::uint8 colour = 245;
    int barWidth = 200;
    int presetButtonWidth = 30;
};

#endif /* PresetBar_h */
