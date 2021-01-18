/*
  ==============================================================================

    CombFilter.h
    Created: 3 Jan 2021 1:01:58pm
    Author:  Aimee Dowell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using namespace juce;

class CombFilter
{
public:
    CombFilter() noexcept {}

    void setSize(const int size)
    {
        if (size != bufferSize)
        {
            bufferIndex = 0;
            buffer.malloc(size);
            bufferSize = size;
        }

        clear();
    }

    void clear() noexcept
    {
        last = 0;
        buffer.clear((size_t) bufferSize);
    }

    float process(const float input, const float damp, const float feedbackLevel) noexcept
    {
        const float output = buffer[bufferIndex];
        last = (output * (1.0f - damp)) + (last * damp);
        float temp = input + (last * feedbackLevel);
        buffer[bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;
        return output;
    }

private:
    
    juce::HeapBlock<float> buffer;
    int bufferSize = 0, bufferIndex = 0;
    float last = 0.0f;

    JUCE_DECLARE_NON_COPYABLE (CombFilter)
};
