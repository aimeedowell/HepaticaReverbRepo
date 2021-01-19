//PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "AudioVisualiserMeter.h"
#include "Equalisation.h"
#include "ModulationDial.h"

ReverbAudioProcessorEditor::ReverbAudioProcessorEditor(ReverbAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, reverbLookAndFeel(CustomLookAndFeel())
, bypassButton(std::make_unique<juce::ImageButton>())
, gainSlider(std::make_unique<juce::Slider>())
, gainSliderLabel(std::make_unique<juce::Label>())
, panSlider(std::make_unique<juce::Slider>())
, leftPanSliderLabel(std::make_unique<juce::Label>())
, rightPanSliderLabel(std::make_unique<juce::Label>())
, wetDrySlider(std::make_unique<juce::Slider>())
, drySliderLabel(std::make_unique<juce::Label>())
, reverbSizeSlider(std::make_unique<juce::Slider>())
, reverbSizeSliderLabel(std::make_unique<juce::Label>())
, preDelaySlider(std::make_unique<juce::Slider>())
, preDelaySliderLabel(std::make_unique<juce::Label>())
, earlyReflectionsSlider(std::make_unique<juce::Slider>())
, earlyReflectionsSliderLabel(std::make_unique<juce::Label>())
, stereoSpread(std::make_unique<ModulationDial>(audioProcessor, "stereoSpreadID"))
, damping(std::make_unique<ModulationDial>(audioProcessor, "dampingID"))
, stereoSpreadLabel(std::make_unique<juce::Label>())
, dampingLabel(std::make_unique<juce::Label>())
, leftAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
, rightAudioMeter(std::make_unique<AudioVisualiserMeter>(*this))
, eqGraph(std::make_unique<Equalisation>(audioProcessor))
, eqGraphLabel(std::make_unique<juce::Label>())
{
    setLookAndFeel(&reverbLookAndFeel);
    
    setSize (830, 350);
    
    AddBypassButton();
    AddGainSlider();
    AddWetDrySlider();
    AddReverbSizeSlider();
    AddPreDelaySlider();
    AddEarlyReflectionsSlider();
    AddModulationSliders();
    AddPanSlider();
    AddAudioVisualiser();
    AddEqualisationGraph();
    
    startTimer(10);
}

ReverbAudioProcessorEditor::~ReverbAudioProcessorEditor()
{
    stopTimer();
    bypassButtonAttachment.reset(); //Delete parameter attachments before component is deleted else the attachment has no idea what it's dettaching from!!!
    bypassButton.reset();
    gainSliderAttachment.reset();
    gainSlider.reset();
    gainSliderLabel.reset();
    panSliderAttachment.reset();
    panSlider.reset();
    leftPanSliderLabel.reset();
    rightPanSliderLabel.reset();
    wetDrySliderAttachment.reset();
    wetDrySlider.reset();
    drySliderLabel.reset();
    reverbSizeSliderAttachment.reset();
    reverbSizeSlider.reset();
    reverbSizeSliderLabel.reset();
    preDelaySliderAttachment.reset();
    preDelaySlider.reset();
    preDelaySliderLabel.reset();
    earlyReflectionsSliderAttachment.reset();
    earlyReflectionsSlider.reset();
    earlyReflectionsSliderLabel.reset();
    stereoSpread.reset();
    damping.reset();
    leftAudioMeter.reset();
    rightAudioMeter.reset();
    eqGraph.reset();
    eqGraphLabel.reset();
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
    
    SetBypassButtonBounds(width, height);
    SetGainSliderBounds(width, height);
    SetPanSliderBounds(width, height);
    SetWetDrySliderBounds(width, height);
    SetReverbSizeSliderBounds(width, height);
    SetPreDelaySliderBounds(width, height);
    SetEarlyReflectionsSliderBounds(width, height);
    SetModulationSliderBounds(width, height);
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
    DrawTitle(g);
}

void ReverbAudioProcessorEditor::DrawTitle(juce::Graphics &g)
{
    juce::Rectangle<float> rect;
    rect.setBounds(getWidth()/2 - 100, 10, 200, 35);
    g.drawImage(title, rect, juce::RectanglePlacement::stretchToFit, false);
}

void ReverbAudioProcessorEditor::AddBypassButton()
{
    juce::Image bypassOffImage = juce::ImageCache::getFromMemory(BinaryData::BypassOff_PNG, BinaryData::BypassOff_PNGSize);
    juce::Image bypassOnImage  = juce::ImageCache::getFromMemory(BinaryData::BypassOn_PNG, BinaryData::BypassOn_PNGSize);
    
    addAndMakeVisible(bypassButton.get());
    bypassButton.get()->addListener(this);
    bypassButton.get()->setImages(false, true, true, bypassOffImage, 0.9f, noColour, bypassOnImage, 0.5f, noColour, bypassOnImage, 1.0f, noColour);
    bypassButton.get()->setClickingTogglesState(true);
    
    bypassButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.GetValueTreeState()
                                                                                                  , "bypassID"
                                                                                                  , *bypassButton.get());
}

void ReverbAudioProcessorEditor::AddGainSlider()
{
    addAndMakeVisible(gainSlider.get());
    addAndMakeVisible(gainSliderLabel.get());
    gainSliderLabel->setInterceptsMouseClicks(false, false);
    gainSliderLabel->setText("Gain", juce::dontSendNotification);
    gainSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    gainSlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider->setDoubleClickReturnValue(true, 0.0);
    gainSlider->addListener(this);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                             , "gainID"
                                                                             , *gainSlider.get());
    
}

void ReverbAudioProcessorEditor::AddPanSlider()
{
    addAndMakeVisible(panSlider.get());
    addAndMakeVisible(leftPanSliderLabel.get());
    addAndMakeVisible(rightPanSliderLabel.get());
    leftPanSliderLabel->setInterceptsMouseClicks(false, false);
    rightPanSliderLabel->setInterceptsMouseClicks(false, false);
    leftPanSliderLabel->setText("Left", juce::NotificationType::dontSendNotification);
    leftPanSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    rightPanSliderLabel->setText("Right", juce::NotificationType::dontSendNotification);
    rightPanSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    panSlider->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    panSlider->setDoubleClickReturnValue(true, 0.0);
    panSlider->addListener(this);
    panSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                , "panningID"
                                                                                                , *panSlider.get());
}

void ReverbAudioProcessorEditor::AddWetDrySlider()
{
    addAndMakeVisible(wetDrySlider.get());
    addAndMakeVisible(drySliderLabel.get());
    drySliderLabel->setInterceptsMouseClicks(false, false);
    drySliderLabel->setText("Mix", juce::dontSendNotification);
    drySliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    wetDrySlider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    wetDrySlider->setDoubleClickReturnValue(true, 0.0);
    wetDrySlider->addListener(this);
    wetDrySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                             , "wetDryID"
                                                                             , *wetDrySlider.get());
    
}

void ReverbAudioProcessorEditor::SetBypassButtonBounds(int width, int height)
{
    bypassButton->setBounds(width - 170, 10 , 200, 20);
}

void ReverbAudioProcessorEditor::SetGainSliderBounds(int width, int height)
{
    gainSlider->setBounds(680, 20 , 60, 300);
    gainSliderLabel->setBounds(690, 310, 40, 30);
}

void ReverbAudioProcessorEditor::SetPanSliderBounds(int width, int height)
{
    panSlider->setBounds(275, 295, 280, 60);
    leftPanSliderLabel->setBounds(240, 320, 40, 30);
    rightPanSliderLabel->setBounds(545, 320, 40, 30);
}

void ReverbAudioProcessorEditor::SetWetDrySliderBounds(int width, int height)
{
    wetDrySlider->setBounds(35, 50 , 60, 280);
    drySliderLabel->setBounds(40, 315, 40, 30);
}

void ReverbAudioProcessorEditor::AddReverbSizeSlider()
{
    addAndMakeVisible(reverbSizeSlider.get());
    addAndMakeVisible(reverbSizeSliderLabel.get());
    reverbSizeSliderLabel->setInterceptsMouseClicks(false, false);
    reverbSizeSliderLabel->setText("Reverb Size", juce::NotificationType::dontSendNotification);
    reverbSizeSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    reverbSizeSlider->addListener(this);
    reverbSizeSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    reverbSizeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , "reverbSizeID"
                                                                                                   , *reverbSizeSlider.get());
}

void ReverbAudioProcessorEditor::AddPreDelaySlider()
{
    addAndMakeVisible(preDelaySlider.get());
    addAndMakeVisible(preDelaySliderLabel.get());
    preDelaySliderLabel->setInterceptsMouseClicks(false, false);
    preDelaySliderLabel->setText("Pre-Delay", juce::NotificationType::dontSendNotification);
    preDelaySliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    preDelaySlider->addListener(this);
    preDelaySlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    preDelaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.GetValueTreeState()
                                                                                                   , "preDelayID"
                                                                                                   , *preDelaySlider.get());
}

void ReverbAudioProcessorEditor::AddEarlyReflectionsSlider()
{
    addAndMakeVisible(earlyReflectionsSlider.get());
    addAndMakeVisible(earlyReflectionsSliderLabel.get());
    earlyReflectionsSliderLabel->setInterceptsMouseClicks(false, false);
    earlyReflectionsSliderLabel->setText("Early Reflections", juce::NotificationType::dontSendNotification);
    earlyReflectionsSliderLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
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

void ReverbAudioProcessorEditor::AddModulationSliders()
{
    addAndMakeVisible(stereoSpread.get());
    addAndMakeVisible(damping.get());
    addAndMakeVisible(stereoSpreadLabel.get());
    addAndMakeVisible(dampingLabel.get());
    stereoSpreadLabel->setInterceptsMouseClicks(false, false);
    dampingLabel->setInterceptsMouseClicks(false, false);
    stereoSpreadLabel->setText("Stereo Spread", juce::NotificationType::dontSendNotification);
    stereoSpreadLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    dampingLabel->setText("Damping", juce::NotificationType::dontSendNotification);
    dampingLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
}

void ReverbAudioProcessorEditor::SetModulationSliderBounds(int width, int height)
{
    damping->setBounds(545, 95, damping->GetModDialWidth(), damping->GetModDialHeight());
    stereoSpread->setBounds(610, 45, stereoSpread->GetModDialWidth(), stereoSpread->GetModDialHeight());
    dampingLabel->setBounds(620, 135, 70, 30);
    stereoSpreadLabel->setBounds(515, 50, 90, 30);
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

void ReverbAudioProcessorEditor::AddEqualisationGraph()
{
    addAndMakeVisible(eqGraphLabel.get());
    addAndMakeVisible(eqGraph.get());
    eqGraphLabel->setInterceptsMouseClicks(false, false);
    eqGraphLabel->setText("Equalisation", juce::NotificationType::dontSendNotification);
    eqGraphLabel->setColour(juce::Label::textColourId, juce::Colour(labelColour, labelColour, labelColour, 0.6f));
    
    eqGraphLabel.get()->setBounds(140, 37, 130, 20);
    eqGraph.get()->setBounds(95, 60, eqGraph.get()->GetEQWidth(), eqGraph.get()->GetEQHeight());
}
