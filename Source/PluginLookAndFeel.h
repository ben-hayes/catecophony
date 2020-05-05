/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    PluginLookAndFeel.h
    Description: Header file for look and feel subclass. Used to style some
      aspects of the plugin.
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
    
    void drawButtonBackground(
        Graphics& g,
        Button& button,
        const Colour& bgColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
    
private:
    void initColours();

    Colour alphaWhite;
    Colour deepBlue;
};