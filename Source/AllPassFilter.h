/*
  ==============================================================================

    AllPassFilter.h
    Created: 3 Jan 2021 1:00:39pm
    Author:  Aimee Dowell

  ==============================================================================
*/

/*
 This class creates a simple all pass filter. The size of the buffer is determined through the delay value of the filter and stores the previous value of the buffer for use in feedback. A clear function has been created to the buffer is initialsed to zero on starting.
 
 */

#pragma once
#include <JuceHeader.h>

using namespace juce;

class AllPassFilter
{
public:
    AllPassFilter() noexcept {}

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
        buffer.clear((size_t) bufferSize);
    }

    float process(const float input, const float feedbackLevel) noexcept
    {
        const float bufferedValue = buffer [bufferIndex];
        float temp = input + (bufferedValue * feedbackLevel);
        buffer [bufferIndex] = temp;
        bufferIndex = (bufferIndex + 1) % bufferSize;
        return bufferedValue - input;
    }

private:
    HeapBlock<float> buffer;
    int bufferSize = 0, bufferIndex = 0;

    JUCE_DECLARE_NON_COPYABLE (AllPassFilter)
};
