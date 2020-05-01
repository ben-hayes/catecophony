/*
  ==============================================================================

    PluginLookAndFeel.cpp
    Created: 27 Apr 2020 2:51:20pm
    Author:  Ben Hayes

  ==============================================================================
*/

#include "PluginLookAndFeel.h"

PluginLookAndFeel::PluginLookAndFeel()
    : alphaWhite((0xa0 << 24) | (0xff << 16) | (0xff << 8) | (0xff)),
      deepBlue((0xff << 24) | (0x21 << 16) | (0x24 << 8) | (0x3d))
{
    initColours();
}

void PluginLookAndFeel::drawComboBox(
    Graphics& g,
    int width,
    int height,
    bool,
    int,
    int,
    int,
    int,
    ComboBox& box)
{
    Rectangle<int> boxBounds (0, 0, width, height);
    g.setColour(box.findColour(ComboBox::outlineColourId));
    g.drawRect(boxBounds, 1.0f);

    Rectangle<int> triangleBounds (width - 18, 0, 12, height);
    g.setColour(box.findColour(ComboBox::arrowColourId)
        .withAlpha(box.isEnabled() ? 0.7f : 0.15f));

    Path trianglePath;
    trianglePath.startNewSubPath(
        triangleBounds.getX() + 0.0f,
        triangleBounds.getCentreY() - 3.0f);
    trianglePath.lineTo(
        triangleBounds.getCentreX() + 0.0f,
        triangleBounds.getCentreY() + 5.0f);
    trianglePath.lineTo(
        triangleBounds.getRight() + 0.0f,
        triangleBounds.getCentreY() - 3.0f);
    trianglePath.lineTo(
        triangleBounds.getX() + 0.0f,
        triangleBounds.getCentreY() - 3.0f);
    g.fillPath(trianglePath);
}

void PluginLookAndFeel::drawButtonBackground(
        Graphics& g,
        Button& button,
        const Colour& bgColour,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    g.setColour(alphaWhite);

    g.drawRect(bounds, 1.0f);
    if (shouldDrawButtonAsDown) g.fillRect(bounds);
}

void PluginLookAndFeel::initColours()
{

    // Window colours
    this->setColour(ResizableWindow::backgroundColourId, deepBlue);

    // Combo Box colours
    this->setColour(ComboBox::outlineColourId, alphaWhite);
    this->setColour(ComboBox::arrowColourId, alphaWhite);

    // Popup Menu colours
    this->setColour(PopupMenu::backgroundColourId, alphaWhite);
    this->setColour(
        PopupMenu::highlightedBackgroundColourId,
        alphaWhite.withAlpha(juce::uint8(0xc0))
    );
    this->setColour(PopupMenu::textColourId, deepBlue);
    this->setColour(PopupMenu::highlightedTextColourId, deepBlue);

    // Slider colours
    this->setColour(Slider::textBoxOutlineColourId, alphaWhite);
    this->setColour(Slider::trackColourId, deepBlue);
    this->setColour(Slider::textBoxHighlightColourId, deepBlue);
    this->setColour(Slider::textBoxTextColourId, alphaWhite);
}