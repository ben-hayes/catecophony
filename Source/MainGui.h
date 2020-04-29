/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "CorpusView.h"
#include "FileDragAndDrop.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainGui  : public Component,
                 public ComboBox::Listener,
                 public Slider::Listener
{
public:
    //==============================================================================
    MainGui (std::unique_ptr<WildcardFileFilter> fileFilter);
    ~MainGui() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setFileDropCallback(std::function<void(const StringArray&)> callback);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<CorpusView> corpus_view;
    std::unique_ptr<FileDragAndDrop> drag_and_drop;
    std::unique_ptr<ComboBox> feature_3;
    std::unique_ptr<ComboBox> feature_2;
    std::unique_ptr<ComboBox> feature_1;
    std::unique_ptr<Slider> feature_1_weight;
    std::unique_ptr<Slider> feature_2_weight;
    std::unique_ptr<Slider> feature_3_weight;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainGui)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

