/*
  ==============================================================================

    LoadingBar.h
    Created: 1 May 2020 6:47:50pm
    Author:  Ben Hayes

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
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~LoadingBar()
    {
    }

    void paint (Graphics& g) override
    {
        if (!hasCallback) 
        {
            stopTimer();
            return;
        }

        auto progress = getProgress();
        if (progress < 0.0f) return;

        auto loadingBarBounds = getLocalBounds().withSizeKeepingCentre(160, 40);
        g.setColour(Colour((0xa0 << 24) | (0xff << 16) | (0xff << 8) | (0xff)));
        g.drawRect(loadingBarBounds);
        g.fillRect(loadingBarBounds.withWidth((int)(160 * progress)));
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

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
        repaint();
    }

private:
    bool hasCallback = false;
    std::function<float()> getProgress;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoadingBar)
};
