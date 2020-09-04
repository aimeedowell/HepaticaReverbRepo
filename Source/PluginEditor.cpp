//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor (ReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , gainSlider(std::make_unique<juce::Slider>())
    , gainSliderLabel(std::make_unique<juce::Label>())
{
    setSize (830, 350);
    
    AddGainSlider();
    AddGainSliderLabel();
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    gainSliderAttachment.reset(); //Delete this before gainSlider is deleted else the attachment has no idea what it's dettaching from!!!
    gainSlider.reset();
    gainSliderLabel.reset();
}

void ReverbAudioProcessorEditor::paint (juce::Graphics &g)
{
    AddCommonPluginBackground(g);
}

void ReverbAudioProcessorEditor::resized()
{
    SetGainSliderBounds();
}

void ReverbAudioProcessorEditor::AddCommonPluginBackground(juce::Graphics &g)
{
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::CommonPluginBackground_PNG, BinaryData::CommonPluginBackground_PNGSize);
    
    g.drawImageWithin(background, 0, 0, getWidth() ,getHeight(), false);
}

void ReverbAudioProcessorEditor::AddGainSlider()
{
    addAndMakeVisible(gainSlider.get());
    gainSlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    gainSlider->setDoubleClickReturnValue(true, 0.0);
    gainSlider->addListener(this);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                             , "gainID"
                                                                             , *gainSlider.get());
    
    SetGainSliderColour();
}

void ReverbAudioProcessorEditor::SetGainSliderColour()
{
    juce::uint8 red = 109;
    juce::uint8 green = 0;
    juce::uint8 blue = 181;
    auto transparent = juce::Colour(red, green, blue, 0.0f);
    
    gainSlider->setColour(juce::Slider::backgroundColourId, juce::Colour(red, green, blue, 0.4f));
    gainSlider->setColour(juce::Slider::thumbColourId, juce::Colour(red, green, blue));
    gainSlider->setColour(juce::Slider::trackColourId, transparent);
}

void ReverbAudioProcessorEditor::AddGainSliderLabel()
{
    juce::uint8 white = 230;
    addAndMakeVisible(gainSliderLabel.get());
    gainSliderLabel->setFont(juce::Font ("Noteworthy", 25.0f, juce::Font::italic));
    gainSliderLabel->setText ("Gain", juce::dontSendNotification);
    gainSliderLabel->setJustificationType(juce::Justification::Flags::centredBottom);
    gainSliderLabel->setColour(juce::Label::textColourId, juce::Colour(white, white, white, 0.6f));
}

void ReverbAudioProcessorEditor::SetGainSliderBounds()
{
    auto width = getWidth();
    auto height = getHeight();
    gainSlider->setBounds(width - 150, 20 , 60, 300);
    gainSliderLabel->setBounds(width - 140, height - 40, 40, 30);
}
