/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    FileDragAndDrop.cpp
    Description: Drag and drop implementation
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
    // Highlight the interface if we're hovering a file
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
    // Only report that we're interested if we have a format reader for the file
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

    // pass the list of files to our callback
    callback(files);
}
