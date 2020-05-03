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
#include <functional>

#include <JuceHeader.h>

#include "FileDragAndDrop.h"
#include "GrainView.h"
#include "LoadingBar.h"
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
                 public Slider::Listener,
                 public Button::Listener
{
public:
    //==============================================================================
    MainGui (AudioProcessorValueTreeState& v, std::unique_ptr<WildcardFileFilter> fileFilter);
    ~MainGui() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setFileDropCallback(std::function<void(const StringArray&)> callback);
    void setAnalyseCallback(std::function<void()> callback);

    void startLoading(std::function<float()> callback);
    void stopLoading();

    void startGrainAnimation(
        std::unique_ptr<Array<Array<float>>> grains,
        std::function<Array<int>()> matchCallback);
    void stopGrainAnimation();
    bool animationIsRunning();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        dryWetAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        lpfCutoffAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        temperatureAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        grainSizeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        hopSizeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>
        window_typeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>
        feature_1Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>
        feature_2Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>
        feature_3Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        matchGainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment>
        relativeMatchingAttachment;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> label3;
    std::unique_ptr<Label> label2;
    std::unique_ptr<LoadingBar> loading_bar;
    std::unique_ptr<ComboBox> feature_3;
    std::unique_ptr<ComboBox> feature_2;
    std::unique_ptr<ComboBox> feature_1;
    std::unique_ptr<Slider> dry_wet;
    std::unique_ptr<Label> label;
    std::unique_ptr<TextButton> analyse;
    std::unique_ptr<Slider> grain_size;
    std::unique_ptr<Slider> hop_size;
    std::unique_ptr<ToggleButton> matchGain;
    std::unique_ptr<GrainView> grainView;
    std::unique_ptr<FileDragAndDrop> drag_and_drop;
    std::unique_ptr<Slider> temperature;
    std::unique_ptr<Label> label4;
    std::unique_ptr<ToggleButton> relativeMatching;
    std::unique_ptr<ComboBox> window_type;
    std::unique_ptr<Slider> lpfCutoff;
    std::unique_ptr<Label> label5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainGui)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

