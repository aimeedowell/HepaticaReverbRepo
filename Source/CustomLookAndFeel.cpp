//
//  CustomLookAndFeel.cpp
//  Hepatica Reverb - VST3
//
//  Created by Aimee Dowell on 08/09/2020.
//  Copyright © 2020 Aimee Dowell. All rights reserved.
//
#include "CustomLookAndFeel.h"
#include <stdio.h>
using namespace juce;

CustomLookAndFeel::CustomLookAndFeel()
: flowerImage(juce::ImageCache::getFromMemory(BinaryData::Hepatica_PNG, BinaryData::Hepatica_PNGSize))
, dotSliderImage(juce::ImageCache::getFromMemory(BinaryData::HepaticaDot_PNG, BinaryData::HepaticaDot_PNGSize))
{
    
}

CustomLookAndFeel::~CustomLookAndFeel()
{
    
}

juce::Font CustomLookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font("Noteworthy", 25.0f, juce::Font::italic);
}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style, Slider& slider)
{
    SetDefaultSliderColour(slider);
    
    if (slider.isBar())
    {
        g.setColour (slider.findColour (Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                          : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
    }
    else
    {
        auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = 8.0f;

        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                                 slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                               slider.isHorizontal() ? startPoint.y : (float) y);
        
        if (slider.isHorizontal() && width > 200)
        {
            SetDottedSliderColours(slider);
            trackWidth = trackWidth * 2.0f;
        }

        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (Slider::backgroundColourId));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        if (!isTwoVal || !isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                         slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

            auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        auto thumbWidth = trackWidth * 3.5f;
        
        if (slider.isHorizontal() && width > 200)
        {
            DrawDottedSlider(g, x, y, trackWidth, width, height, sliderPos);
            
            thumbWidth = trackWidth;
        }

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        if (! isTwoVal)
        {
            g.setColour (slider.findColour (Slider::thumbColourId));
            g.fillEllipse (Rectangle<float> (static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre (isThreeVal ? thumbPoint : maxPoint));
        }
    }
}

void CustomLookAndFeel::SetDefaultSliderColour(juce::Slider &slider)
{
    juce::uint8 red = 109;
    juce::uint8 green = 0;
    juce::uint8 blue = 181;
    auto transparent = juce::Colour(red, green, blue, 0.0f);
    slider.setColour(Slider::backgroundColourId, juce::Colour(red, green, blue, 0.6f));
    slider.setColour(Slider::thumbColourId, juce::Colour(174, 119, 212));
    slider.setColour(Slider::trackColourId, transparent);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}

void CustomLookAndFeel::DrawDottedSlider(juce::Graphics &g, int x, int y, int trackWidth, int width, int height, float sliderPos)
{
    auto startPos = (float) x + (float) width * 0.5f;
    auto verticalFlip = AffineTransform::rotation(3.1415926536f, startPos, float(y + 19));
    float distance = 0;
    float sliderRatio = 0;
    g.setOpacity(1.0f);
    
    if (sliderPos >= 140)
    {
        distance = sliderPos - startPos;
        sliderRatio = distance/startPos;
        
        g.drawImage(dotSliderImage, startPos, 22, startPos * sliderRatio, trackWidth, 0, 0, dotSliderImage.getWidth() * sliderRatio, dotSliderImage.getHeight());
    }
    else if(sliderPos <= 140)
    {
        distance = startPos - sliderPos;
        sliderRatio = distance/startPos;

        g.addTransform(verticalFlip);
        
        g.drawImage(dotSliderImage, startPos, 0, startPos * sliderRatio, trackWidth, 0, 0, dotSliderImage.getWidth() * sliderRatio, dotSliderImage.getHeight());
        
        g.addTransform(verticalFlip);

    }
}

void CustomLookAndFeel::SetDottedSliderColours(juce::Slider &slider)
{
    juce::uint8 red = 197;
    juce::uint8 green = 4;
    juce::uint8 blue = 222;
    auto transparent = juce::Colour(red, green, blue, 0.0f);
    slider.setColour(Slider::backgroundColourId, juce::Colour(red, green, blue, 0.6f));
    slider.setColour(Slider::thumbColourId, juce::Colour(245, 245, 245));
    slider.setColour(Slider::trackColourId, transparent);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}

void CustomLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    uint8 white = 245;
    slider.setColour(Slider::rotarySliderFillColourId, Colour(111, 11, 224));
    slider.setColour(Slider::rotarySliderOutlineColourId, Colour(white, white, white, 0.7f));
    slider.setColour(Slider::thumbColourId, Colour(uint8(111), uint8(11), uint8(224), 0.0f));
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
    auto fill    = slider.findColour (Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);

    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (3.0f, radius * 0.3f);
    auto arcRadius = radius - lineW * 0.3f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    if (slider.isEnabled())
    {
        Path valueArc;
        
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        
        g.setColour (fill);
        g.strokePath (valueArc, PathStrokeType (lineW * 4.0f, PathStrokeType::curved, PathStrokeType::rounded));
        
        g.setColour (outline);
        g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
        
        auto transform = AffineTransform::rotation(toAngle, centreX, centreY);
        
        float opacity;
        opacity = slider.isMouseOverOrDragging() ? 1.0f : 0.7f;
        g.setOpacity(opacity);
        
        g.addTransform(transform);
        g.drawImageWithin(flowerImage, width/9.5, height/9.5, width/1.25, height/1.25, RectanglePlacement::stretchToFit);
    
    }
    
 
    auto thumbWidth = lineW;
    Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));

    g.setColour (slider.findColour (Slider::thumbColourId));
    g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
}
