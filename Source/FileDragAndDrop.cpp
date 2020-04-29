/*
  ==============================================================================

    FileDragAndDrop.cpp
    Created: 27 Apr 2020 4:28:32pm
    Author:  Ben Hayes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileDragAndDrop.h"

//==============================================================================
FileDragAndDrop::FileDragAndDrop(std::unique_ptr<WildcardFileFilter> fileFilter)
    : fileFilter(std::move(fileFilter))
{
}

FileDragAndDrop::~FileDragAndDrop()
{
}

void FileDragAndDrop::setCallback(
    std::function<void(const StringArray&)>& givenCallback)
{
    callback = givenCallback;
    callbackSet = true;
}

void FileDragAndDrop::paint (Graphics& g)
{
    if (filesHovering)
    {
        g.fillAll(Colour(
            (0x22 << 24) | (0xff << 16) | (0xff << 8) | (0xff)
        ));
    } else
    {
        g.fillAll(Colour(
            (0x00 << 24) | (0xff << 16) | (0xff << 8) | (0xff)
        ));
    }
}

void FileDragAndDrop::resized()
{
}

bool FileDragAndDrop::isInterestedInFileDrag(
    const StringArray& files)
{
    for (auto file : files)
    {
        if (!fileFilter->isFileSuitable(file))
        {
            return false;
        }
    }
    return true;
}

void FileDragAndDrop::fileDragEnter(
    const StringArray& files,
    int x,
    int y)
{
    filesHovering = true;
    repaint();
}

void FileDragAndDrop::fileDragMove(
    const StringArray& files,
    int x,
    int y)
{
}

void FileDragAndDrop::fileDragExit(
    const StringArray& files)
{
    filesHovering = false;
    repaint();
}

void FileDragAndDrop::filesDropped(
    const StringArray& files,
    int x,
    int y)
{
    filesHovering = false;
    repaint();

    if(!callbackSet) return;

    callback(files);
}
