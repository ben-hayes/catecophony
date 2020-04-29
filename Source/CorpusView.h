/*
  ==============================================================================

    CorpusView.h
    Created: 27 Apr 2020 4:12:07pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CorpusView    : public Component
{
public:
    CorpusView();
    ~CorpusView();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CorpusView)
};
