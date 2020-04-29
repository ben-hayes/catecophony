/*
  ==============================================================================

    FileDragAndDrop.h
    Created: 27 Apr 2020 4:28:32pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once

#include <functional>

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FileDragAndDrop    : public Component,
                           public FileDragAndDropTarget
{
public:
    FileDragAndDrop(std::unique_ptr<WildcardFileFilter> fileFilter);

    ~FileDragAndDrop();

    void setCallback(std::function<void(const StringArray&)>& givenCallback);

    void paint (Graphics& g) override;

    void resized() override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void fileDragEnter(const StringArray& files, int x, int y) override;
    void fileDragMove(const StringArray& files, int x, int y) override;
    void fileDragExit(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileDragAndDrop)

    std::unique_ptr<WildcardFileFilter> fileFilter;

    std::function<void(const StringArray&)> callback;
    bool callbackSet = false;

    bool filesHovering = false;
};