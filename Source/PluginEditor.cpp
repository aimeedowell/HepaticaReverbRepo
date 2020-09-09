//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "AudioVisualiserMeter.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(ReverbAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, reverbLookAndFeel(CustomLookAndFeel())
, gainSlider(std::make_unique<juce::Slider>())
, gainSliderLabel(std::make_unique<juce::Label>())
, reverbSizeSlider(std::make_unique<juce::Slider>())
, reverbSizeSliderLabel(std::make_unique<juce::Label>())
, preDelaySlider(std::make_unique<juce::Slider>())
, preDelaySliderLabel(std::make_unique<juce::Label>())
, earlyReflectionsSlider(std::make_unique<juce::Slider>())
, earlyReflectionsSliderLabel(std::make_unique<juce::Label>())
, leftAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
, rightAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
{
    setLookAndFeel(&reverbLookAndFeel);
    
    setSize (830, 350);
    
    AddGainSlider();
    AddReverbSizeSlider();
    AddPreDelaySlider();
    AddEarlyReflectionsSlider();
    AddAudioVisualiser();
    
    startTimer(10);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    stopTimer();
    gainSliderAttachment.reset(); //Delete parameter attachments before component is deleted else the attachment has no idea what it's dettaching from!!!
    gainSlider.reset();
    gainSliderLabel.reset();
    reverbSizeSliderAttachment.reset();
    reverbSizeSlider.reset();
    preDelaySliderAttachment.reset();
    preDelaySlider.reset();
    earlyReflectionsSliderAttachment.reset();
    earlyReflectionsSlider.reset();
    leftAudioMeter.reset();
    rightAudioMeter.reset();
    setLookAndFeel(nullptr);
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
    SetReverbSizeSliderBounds(width, height);
    SetPreDelaySliderBounds(width, height);
    SetEarlyReflectionsSliderBounds(width, height);
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
    addAndMakeVisible(gainSliderLabel.get());
    gainSliderLabel->setText("Gain", juce::dontSendNotification);
    gainSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    gainSlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider->setDoubleClickReturnValue(true, 0.0);
    gainSlider->addListener(this);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                             , "gainID"
                                                                             , *gainSlider.get());
    
}

{
}

void ReverbAudioProcessorEditor::SetGainSliderBounds(int width, int height)
{
    gainSlider->setBounds(680, 20 , 60, 300);
    gainSliderLabel->setBounds(690, 310, 40, 30);
}

void ReverbAudioProcessorEditor::AddReverbSizeSlider()
{
    juce::uint8 white = 245;
    addAndMakeVisible(reverbSizeSlider.get());
    addAndMakeVisible(reverbSizeSliderLabel.get());
    reverbSizeSliderLabel->setText("Reverb Size", juce::NotificationType::dontSendNotification);
    reverbSizeSliderLabel->setColour(juce::Label::textColourId, juce::Colour(white, white, white, 0.6f));
    reverbSizeSlider->addListener(this);
    reverbSizeSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    reverbSizeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , "reverbSizeID"
                                                                                                   , *reverbSizeSlider.get());
}

void ReverbAudioProcessorEditor::AddPreDelaySlider()
{
    juce::uint8 white = 245;
    addAndMakeVisible(preDelaySlider.get());
    addAndMakeVisible(preDelaySliderLabel.get());
    preDelaySliderLabel->setText("Pre-Delay", juce::NotificationType::dontSendNotification);
    preDelaySliderLabel->setColour(juce::Label::textColourId, juce::Colour(white, white, white, 0.6f));
    preDelaySlider->addListener(this);
    preDelaySlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    preDelaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , "preDelayID"
                                                                                                   , *preDelaySlider.get());
}

void ReverbAudioProcessorEditor::AddEarlyReflectionsSlider()
{
    juce::uint8 white = 245;
    addAndMakeVisible(earlyReflectionsSlider.get());
    addAndMakeVisible(earlyReflectionsSliderLabel.get());
    earlyReflectionsSliderLabel->setText("Early Reflections", juce::NotificationType::dontSendNotification);
    earlyReflectionsSliderLabel->setColour(juce::Label::textColourId, juce::Colour(white, white, white, 0.6f));
    earlyReflectionsSlider->addListener(this);
    earlyReflectionsSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    earlyReflectionsSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , "earlyReflectionsID"
                                                                                                   , *earlyReflectionsSlider.get());
}

void ReverbAudioProcessorEditor::SetReverbSizeSliderBounds(int width, int height)
{
    reverbSizeSlider->setBounds(280, 45 , 270, 270);
    reverbSizeSliderLabel->setBounds(375, 160 , 270, 270);
}

void ReverbAudioProcessorEditor::SetPreDelaySliderBounds(int width, int height)
{
    preDelaySlider->setBounds(545, 170, 150, 150);
    preDelaySliderLabel->setBounds(580, 240, 150, 150);
}

void ReverbAudioProcessorEditor::SetEarlyReflectionsSliderBounds(int width, int height)
{
    earlyReflectionsSlider->setBounds(135, 170, 150, 150);
    earlyReflectionsSliderLabel->setBounds(150, 240, 150, 150);
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
