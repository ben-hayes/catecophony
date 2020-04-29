/*
  ==============================================================================

    PluginLookAndFeel.h
    Created: 27 Apr 2020 2:51:20pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PluginLookAndFeel : public LookAndFeel_V4
{
public:
    PluginLookAndFeel();

    void drawComboBox(
        Graphics& g,
        int width,
        int height,
        bool,
        int,
        int,
        int,
        int,
        ComboBox& box) override;
    
private:
    void initColours();
};