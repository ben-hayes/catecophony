/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    LoadingBar.h
    Description: A simple loading bar component
  ==============================================================================
*/

#pragma once
#include <functional>

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoadingBar    : public Component,
                      public Timer
{
public:
    LoadingBar()
    {
    }

    ~LoadingBar()
    {
    }

    void paint (Graphics& g) override
    {
        // Stop if we don't have anyone to ask about progress
        if (!hasCallback) 
        {
            stopTimer();
            return;
        }

        auto progress = getProgress();
        // And stop if we're finished
        if (progress < 0.0f)
        {
            clearCallback();
            return;
        }

        // Otherwise, simply draw the rectangle and fill it the appropriate
        // amount
        auto loadingBarBounds = getLocalBounds().withSizeKeepingCentre(160, 40);
        g.setColour(Colour((0xa0 << 24) | (0xff << 16) | (0xff << 8) | (0xff)));
        g.drawRect(loadingBarBounds);
        g.fillRect(loadingBarBounds.withWidth((int)(160 * progress)));
    }

    void resized() override
    {
    }

    void setPaintCallback(std::function<float()> callback)
    {
        getProgress = callback;
        hasCallback = true;
        startTimer(66);
    }

    void clearCallback()
    {
        hasCallback = false;
    }

    void timerCallback() override
    {
        // Animate
        repaint();
    }

private:
    bool hasCallback = false;
    std::function<float()> getProgress;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadingBar)
};
