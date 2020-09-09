//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "AudioVisualiserMeter.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(ReverbAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, gainSlider(std::make_unique<juce::Slider>())
, gainSliderLabel(std::make_unique<juce::Label>())
, leftAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
, rightAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
{
    setSize (830, 350);
    
    AddGainSlider();
    AddGainSliderLabel();
    AddAudioVisualiser();
    
    startTimer(10);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    stopTimer();
    gainSliderAttachment.reset(); //Delete this before gainSlider is deleted else the attachment has no idea what it's dettaching from!!!
    gainSlider.reset();
    gainSliderLabel.reset();
    leftAudioMeter.reset();
    rightAudioMeter.reset();
}

void ReverbAudioProcessorEditor::paint(juce::Graphics &g)
{
    AddCommonPluginBackground(g);
}

void ReverbAudioProcessorEditor::resized()
{
    auto width = getWidth();
    auto height = getHeight();
    
    SetGainSliderBounds(width, height);
}

void ReverbAudioProcessorEditor::timerCallback()
{
    auto amplitudeVector = audioProcessor.GetAmplitudeLevel();
    
    leftAudioMeter.get()->AddAmplitudeToBuffer(amplitudeVector[0]);
    rightAudioMeter.get()->AddAmplitudeToBuffer(amplitudeVector[1]);
}

void ReverbAudioProcessorEditor::AddCommonPluginBackground(juce::Graphics &g)
{
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::CommonPluginBackground_PNG, BinaryData::CommonPluginBackground_PNGSize);
    
    g.drawImageWithin(background, 0, 0, getWidth() ,getHeight(), false);
}

void ReverbAudioProcessorEditor::AddGainSlider()
{
    addAndMakeVisible(gainSlider.get());
    gainSlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider->setDoubleClickReturnValue(true, 0.0);
    gainSlider->addListener(this);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                             , "gainID"
                                                                             , *gainSlider.get());
    
}

void ReverbAudioProcessorEditor::AddGainSliderLabel()
{
    juce::uint8 white = 230;
    addAndMakeVisible(gainSliderLabel.get());
    gainSliderLabel->setText ("Gain", juce::dontSendNotification);
    gainSliderLabel->setJustificationType(juce::Justification::Flags::centredBottom);
    gainSliderLabel->setColour(juce::Label::textColourId, juce::Colour(white, white, white, 0.6f));
}

void ReverbAudioProcessorEditor::SetGainSliderBounds(int width, int height)
{
    gainSlider->setBounds(width - 150, 20 , 60, 300);
    gainSliderLabel->setBounds(width - 140, height - 40, 40, 30);
}

void ReverbAudioProcessorEditor::AddAudioVisualiser()
{
    addAndMakeVisible(leftAudioMeter.get());
    addAndMakeVisible(rightAudioMeter.get());
}

void ReverbAudioProcessorEditor::SetAudioVisualiserBounds(AudioVisualiserMeter *meter)
{
    auto yPosition = meter->GetYPosition();
    auto meterWidth = meter->GetWidth();
    auto meterHeight = meter->GetHeight();
    auto meterPosition = 0;

    if (meter == leftAudioMeter.get())
        meterPosition = 110;
    else if (meter == rightAudioMeter.get())
        meterPosition = 90;
    
    meter->setBounds(getWidth() - meterPosition, 30 + yPosition, meterWidth, meterHeight);
}
