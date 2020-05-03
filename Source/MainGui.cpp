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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainGui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainGui::MainGui (AudioProcessorValueTreeState& v, std::unique_ptr<WildcardFileFilter> fileFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label3.reset (new Label ("new label",
                             TRANS("TEMPERATURE")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font ("Futura", 14.20f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colour (0xa0ff7c7c));
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (8, 347, 132, 40);

    label2.reset (new Label ("new label",
                             TRANS("MATCH MAGNITUDE")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font ("Futura", 10.00f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
    label2->setJustificationType (Justification::centredRight);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colour (0xa0ff7c7c));
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (8, 440, 116, 40);

    loading_bar.reset (new LoadingBar());
    addAndMakeVisible (loading_bar.get());
    loading_bar->setName ("Loading Bar");

    loading_bar->setBounds (152, 0, 648, 600);

    feature_3.reset (new ComboBox ("Feature #3"));
    addAndMakeVisible (feature_3.get());
    feature_3->setEditableText (false);
    feature_3->setJustificationType (Justification::centredLeft);
    feature_3->setTextWhenNothingSelected (String());
    feature_3->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    feature_3->addItem (TRANS("None"), 1);
    feature_3->addItem (TRANS("Dissonance"), 2);
    feature_3->addItem (TRANS("F0"), 3);
    feature_3->addItem (TRANS("Inharmonicity"), 4);
    feature_3->addItem (TRANS("MFCC"), 5);
    feature_3->addItem (TRANS("Odd:even Harmonic Ratio"), 6);
    feature_3->addItem (TRANS("Pitch Salience"), 7);
    feature_3->addItem (TRANS("RMS"), 8);
    feature_3->addItem (TRANS("Spectral Centroid"), 9);
    feature_3->addItem (TRANS("Spectral Complexity"), 10);
    feature_3->addItem (TRANS("Spectral Contrast"), 11);
    feature_3->addItem (TRANS("Spectral Flatness"), 12);
    feature_3->addItem (TRANS("Spectral Peaks"), 13);
    feature_3->addItem (TRANS("Spectral Roll-off"), 14);
    feature_3->addItem (TRANS("Strong Peak Ratio"), 15);
    feature_3->addItem (TRANS("Zero Crossing Rate"), 16);
    feature_3->addListener (this);

    feature_3->setBounds (8, 236, 136, 16);

    feature_2.reset (new ComboBox ("Feature #2"));
    addAndMakeVisible (feature_2.get());
    feature_2->setEditableText (false);
    feature_2->setJustificationType (Justification::centredLeft);
    feature_2->setTextWhenNothingSelected (String());
    feature_2->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    feature_2->addItem (TRANS("None"), 1);
    feature_2->addItem (TRANS("Dissonance"), 2);
    feature_2->addItem (TRANS("F0"), 3);
    feature_2->addItem (TRANS("Inharmonicity"), 4);
    feature_2->addItem (TRANS("MFCC"), 5);
    feature_2->addItem (TRANS("Odd:even Harmonic Ratio"), 6);
    feature_2->addItem (TRANS("Pitch Salience"), 7);
    feature_2->addItem (TRANS("RMS"), 8);
    feature_2->addItem (TRANS("Spectral Centroid"), 9);
    feature_2->addItem (TRANS("Spectral Complexity"), 10);
    feature_2->addItem (TRANS("Spectral Contrast"), 11);
    feature_2->addItem (TRANS("Spectral Flatness"), 12);
    feature_2->addItem (TRANS("Spectral Peaks"), 13);
    feature_2->addItem (TRANS("Spectral Roll-off"), 14);
    feature_2->addItem (TRANS("Strong Peak Ratio"), 15);
    feature_2->addItem (TRANS("Zero Crossing Rate"), 16);
    feature_2->addListener (this);

    feature_2->setBounds (8, 212, 136, 16);

    feature_1.reset (new ComboBox ("Feature #1"));
    addAndMakeVisible (feature_1.get());
    feature_1->setEditableText (false);
    feature_1->setJustificationType (Justification::centredLeft);
    feature_1->setTextWhenNothingSelected (String());
    feature_1->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    feature_1->addItem (TRANS("Dissonance"), 1);
    feature_1->addItem (TRANS("F0"), 2);
    feature_1->addItem (TRANS("Inharmonicity"), 3);
    feature_1->addItem (TRANS("MFCC"), 4);
    feature_1->addItem (TRANS("Odd:even Harmonic Ratio"), 5);
    feature_1->addItem (TRANS("Pitch Salience"), 6);
    feature_1->addItem (TRANS("RMS"), 7);
    feature_1->addItem (TRANS("Spectral Centroid"), 8);
    feature_1->addItem (TRANS("Spectral Complexity"), 9);
    feature_1->addItem (TRANS("Spectral Contrast"), 10);
    feature_1->addItem (TRANS("Spectral Flatness"), 11);
    feature_1->addItem (TRANS("Spectral Peaks"), 12);
    feature_1->addItem (TRANS("Spectral Roll-off"), 13);
    feature_1->addItem (TRANS("Strong Peak Ratio"), 14);
    feature_1->addItem (TRANS("Zero Crossing Rate"), 15);
    feature_1->addListener (this);

    feature_1->setBounds (8, 188, 136, 16);

    dry_wet.reset (new Slider ("Dry/Wet"));
    addAndMakeVisible (dry_wet.get());
    dry_wet->setRange (0, 1, 0.01);
    dry_wet->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    dry_wet->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dry_wet->setColour (Slider::backgroundColourId, Colour (0xffff7c7c));
    dry_wet->setColour (Slider::thumbColourId, Colour (0xff88e1f2));
    dry_wet->setColour (Slider::trackColourId, Colour (0xffff7c7c));
    dry_wet->setColour (Slider::rotarySliderFillColourId, Colour (0xffff7c7c));
    dry_wet->setColour (Slider::rotarySliderOutlineColourId, Colour (0xffff7c7c));
    dry_wet->addListener (this);

    dry_wet->setBounds (76, 528, 76, 70);

    label.reset (new Label ("new label",
                            TRANS("DRY\n"
                            "WET")));
    addAndMakeVisible (label.get());
    label->setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colour (0xa0ff7c7c));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (76, 496, 76, 48);

    analyse.reset (new TextButton ("Analyse Corpus"));
    addAndMakeVisible (analyse.get());
    analyse->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    analyse->addListener (this);

    analyse->setBounds (8, 264, 136, 24);

    grain_size.reset (new Slider ("Grain Size"));
    addAndMakeVisible (grain_size.get());
    grain_size->setRange (5, 13, 1);
    grain_size->setSliderStyle (Slider::LinearBar);
    grain_size->setTextBoxStyle (Slider::TextBoxLeft, true, 80, 20);
    grain_size->setColour (Slider::textBoxOutlineColourId, Colour (0xa0ffffff));
    grain_size->addListener (this);
    grain_size->setSkewFactor (2);

    grain_size->setBounds (8, 136, 64, 16);

    hop_size.reset (new Slider ("Hop Size"));
    addAndMakeVisible (hop_size.get());
    hop_size->setRange (4, 12, 1);
    hop_size->setSliderStyle (Slider::LinearBar);
    hop_size->setTextBoxStyle (Slider::TextBoxLeft, true, 80, 20);
    hop_size->setColour (Slider::textBoxOutlineColourId, Colour (0xa0ffffff));
    hop_size->addListener (this);
    hop_size->setSkewFactor (2);

    hop_size->setBounds (80, 136, 64, 16);

    matchGain.reset (new ToggleButton ("Match Grain Magnitude"));
    addAndMakeVisible (matchGain.get());
    matchGain->setButtonText (String());
    matchGain->addListener (this);
    matchGain->setToggleState (true, dontSendNotification);

    matchGain->setBounds (120, 448, 32, 24);

    grainView.reset (new GrainView());
    addAndMakeVisible (grainView.get());
    grainView->setName ("Grain View");

    grainView->setBounds (152, 0, 648, 600);

    drag_and_drop.reset (new FileDragAndDrop (std::move(fileFilter)));
    addAndMakeVisible (drag_and_drop.get());
    drag_and_drop->setName ("Drag And Drop");

    drag_and_drop->setBounds (152, 0, 648, 600);

    temperature.reset (new Slider ("Temperature"));
    addAndMakeVisible (temperature.get());
    temperature->setRange (0, 1, 0.001);
    temperature->setSliderStyle (Slider::LinearHorizontal);
    temperature->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    temperature->setColour (Slider::backgroundColourId, Colour (0xffff7c7c));
    temperature->setColour (Slider::thumbColourId, Colour (0xff88e1f2));
    temperature->setColour (Slider::trackColourId, Colour (0xffff7c7c));
    temperature->setColour (Slider::textBoxOutlineColourId, Colour (0xa0ffffff));

    temperature->setBounds (4, 368, 144, 35);

    label4.reset (new Label ("new label",
                             TRANS("RELATIVE MATCHING")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font ("Futura", 10.00f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
    label4->setJustificationType (Justification::centredRight);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colour (0xa0ff7c7c));
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (8, 416, 116, 40);

    relativeMatching.reset (new ToggleButton ("Relative Matching"));
    addAndMakeVisible (relativeMatching.get());
    relativeMatching->setButtonText (String());
    relativeMatching->addListener (this);
    relativeMatching->setToggleState (true, dontSendNotification);

    relativeMatching->setBounds (120, 424, 32, 24);

    feature_4.reset (new ComboBox ("Feature #1"));
    addAndMakeVisible (feature_4.get());
    feature_4->setEditableText (false);
    feature_4->setJustificationType (Justification::centredLeft);
    feature_4->setTextWhenNothingSelected (String());
    feature_4->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    feature_4->addItem (TRANS("Dissonance"), 1);
    feature_4->addItem (TRANS("F0"), 2);
    feature_4->addItem (TRANS("Inharmonicity"), 3);
    feature_4->addItem (TRANS("MFCC"), 4);
    feature_4->addItem (TRANS("Odd:even Harmonic Ratio"), 5);
    feature_4->addItem (TRANS("Pitch Salience"), 6);
    feature_4->addItem (TRANS("RMS"), 7);
    feature_4->addItem (TRANS("Spectral Centroid"), 8);
    feature_4->addItem (TRANS("Spectral Complexity"), 9);
    feature_4->addItem (TRANS("Spectral Contrast"), 10);
    feature_4->addItem (TRANS("Spectral Flatness"), 11);
    feature_4->addItem (TRANS("Spectral Peaks"), 12);
    feature_4->addItem (TRANS("Spectral Roll-off"), 13);
    feature_4->addItem (TRANS("Strong Peak Ratio"), 14);
    feature_4->addItem (TRANS("Zero Crossing Rate"), 15);
    feature_4->addListener (this);

    feature_4->setBounds (8, 72, 136, 16);

    dry_wet2.reset (new Slider ("Dry/Wet"));
    addAndMakeVisible (dry_wet2.get());
    dry_wet2->setRange (0, 1, 0.01);
    dry_wet2->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    dry_wet2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    dry_wet2->setColour (Slider::backgroundColourId, Colour (0xffff7c7c));
    dry_wet2->setColour (Slider::thumbColourId, Colour (0xff88e1f2));
    dry_wet2->setColour (Slider::trackColourId, Colour (0xffff7c7c));
    dry_wet2->setColour (Slider::rotarySliderFillColourId, Colour (0xffff7c7c));
    dry_wet2->setColour (Slider::rotarySliderOutlineColourId, Colour (0xffff7c7c));
    dry_wet2->addListener (this);

    dry_wet2->setBounds (0, 528, 76, 70);

    label5.reset (new Label ("new label",
                             TRANS("INPUT\n"
                             "GAIN")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (Label::textColourId, Colour (0xa0ff7c7c));
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (0, 496, 76, 48);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    dryWetAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
        v,
        "drywet",
        *dry_wet));
    temperatureAttachment.reset(
        new AudioProcessorValueTreeState::SliderAttachment(
            v,
            "temperature",
            *temperature));
    grainSizeAttachment.reset(
        new AudioProcessorValueTreeState::SliderAttachment(
            v,
            "grainSize",
            *grain_size));
    hopSizeAttachment.reset(
        new AudioProcessorValueTreeState::SliderAttachment(
            v,
            "hopSize",
            *hop_size));
    feature_1Attachment.reset(
        new AudioProcessorValueTreeState::ComboBoxAttachment(
            v,
            "feature_1",
            *feature_1));
    feature_2Attachment.reset(
        new AudioProcessorValueTreeState::ComboBoxAttachment(
            v,
            "feature_2",
            *feature_2));
    feature_3Attachment.reset(
        new AudioProcessorValueTreeState::ComboBoxAttachment(
            v,
            "feature_3",
            *feature_3));
    matchGainAttachment.reset(
        new AudioProcessorValueTreeState::ButtonAttachment(
            v,
            "matchGain",
            *matchGain));

    grain_size->textFromValueFunction = [](double val) {
        return String((int)powf(2.0f, val + 1));
    };
    hop_size->textFromValueFunction = [](double val) {
        return String((int)powf(2.0f, val + 1));
    };
    //[/Constructor]
}

MainGui::~MainGui()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    dryWetAttachment = nullptr;
    temperatureAttachment = nullptr;
    grainSizeAttachment = nullptr;
    hopSizeAttachment = nullptr;
    feature_1Attachment = nullptr;
    feature_2Attachment = nullptr;
    feature_3Attachment = nullptr;
    matchGainAttachment = nullptr;
    //[/Destructor_pre]

    label3 = nullptr;
    label2 = nullptr;
    loading_bar = nullptr;
    feature_3 = nullptr;
    feature_2 = nullptr;
    feature_1 = nullptr;
    dry_wet = nullptr;
    label = nullptr;
    analyse = nullptr;
    grain_size = nullptr;
    hop_size = nullptr;
    matchGain = nullptr;
    grainView = nullptr;
    drag_and_drop = nullptr;
    temperature = nullptr;
    label4 = nullptr;
    relativeMatching = nullptr;
    feature_4 = nullptr;
    dry_wet2 = nullptr;
    label5 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainGui::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff21243d));

    {
        int x = 0, y = 0, width = 152, height = 300;
        Colour fillColour = Colour (0xffff7c7c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 0, y = 300, width = 152, height = 300;
        Colour fillColour = Colour (0xffffd082);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 4, y = 304, width = 292, height = 30;
        String text (TRANS("OUTPUT"));
        Colour fillColour = Colour (0xffff7c7c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 24.10f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 4, y = 4, width = 292, height = 30;
        String text (TRANS("SYNTHESIS"));
        Colour fillColour = Colour (0xff88e1f2);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 24.10f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 8, y = 168, width = 208, height = 30;
        String text (TRANS("FEATURE"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 156, y = 4, width = 292, height = 30;
        String text (TRANS("CORPUS"));
        Colour fillColour = Colour (0xffffd082);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 24.10f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 304, y = 570, width = 496, height = 30;
        String text (TRANS("C A T E C O P H O N Y"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 24.10f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.309f));
        g.drawText (text, x, y, width, height,
                    Justification::topRight, true);
    }

    {
        int x = 8, y = 100, width = 96, height = 20;
        String text (TRANS("GRAIN"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 8, y = 116, width = 96, height = 20;
        String text (TRANS("SIZE"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 80, y = 100, width = 96, height = 20;
        String text (TRANS("HOP"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 80, y = 116, width = 96, height = 20;
        String text (TRANS("SIZE"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    {
        int x = 8, y = 56, width = 208, height = 30;
        String text (TRANS("WINDOW TYPE"));
        Colour fillColour = Colour (0xa0ffffff);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font ("Futura", 14.80f, Font::plain).withTypefaceStyle ("Medium").withExtraKerningFactor (0.147f));
        g.drawText (text, x, y, width, height,
                    Justification::topLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainGui::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainGui::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == feature_3.get())
    {
        //[UserComboBoxCode_feature_3] -- add your combo box handling code here..
        //[/UserComboBoxCode_feature_3]
    }
    else if (comboBoxThatHasChanged == feature_2.get())
    {
        //[UserComboBoxCode_feature_2] -- add your combo box handling code here..
        //[/UserComboBoxCode_feature_2]
    }
    else if (comboBoxThatHasChanged == feature_1.get())
    {
        //[UserComboBoxCode_feature_1] -- add your combo box handling code here..
        //[/UserComboBoxCode_feature_1]
    }
    else if (comboBoxThatHasChanged == feature_4.get())
    {
        //[UserComboBoxCode_feature_4] -- add your combo box handling code here..
        //[/UserComboBoxCode_feature_4]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MainGui::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == dry_wet.get())
    {
        //[UserSliderCode_dry_wet] -- add your slider handling code here..
        //[/UserSliderCode_dry_wet]
    }
    else if (sliderThatWasMoved == grain_size.get())
    {
        //[UserSliderCode_grain_size] -- add your slider handling code here..
        //[/UserSliderCode_grain_size]
    }
    else if (sliderThatWasMoved == hop_size.get())
    {
        //[UserSliderCode_hop_size] -- add your slider handling code here..
        //[/UserSliderCode_hop_size]
    }
    else if (sliderThatWasMoved == dry_wet2.get())
    {
        //[UserSliderCode_dry_wet2] -- add your slider handling code here..
        //[/UserSliderCode_dry_wet2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MainGui::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == analyse.get())
    {
        //[UserButtonCode_analyse] -- add your button handler code here..
        //[/UserButtonCode_analyse]
    }
    else if (buttonThatWasClicked == matchGain.get())
    {
        //[UserButtonCode_matchGain] -- add your button handler code here..
        //[/UserButtonCode_matchGain]
    }
    else if (buttonThatWasClicked == relativeMatching.get())
    {
        //[UserButtonCode_relativeMatching] -- add your button handler code here..
        //[/UserButtonCode_relativeMatching]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MainGui::setFileDropCallback(
    std::function<void(const StringArray&)> callback)
{
    drag_and_drop->setCallback(callback);
}

void MainGui::setAnalyseCallback(std::function<void()> callback)
{
    analyse->onClick = callback;
}

void MainGui::startLoading(std::function<float()> callback)
{
    loading_bar->setPaintCallback(callback);
}

void MainGui::stopLoading()
{
    loading_bar->clearCallback();
}

void MainGui::startGrainAnimation(
    std::unique_ptr<Array<Array<float>>> grains,
    std::function<Array<int>()> matchCallback)
{
    grainView->setGrainCoords(std::move(grains));
    grainView->setMatchCallback(matchCallback);
}

void MainGui::stopGrainAnimation()
{
    grainView->stopAnimation();
}

bool MainGui::animationIsRunning()
{
    return grainView->animationIsRunning();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainGui" componentName=""
                 parentClasses="public Component" constructorParams="AudioProcessorValueTreeState&amp; v, std::unique_ptr&lt;WildcardFileFilter&gt; fileFilter"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff21243d">
    <RECT pos="0 0 152 300" fill="solid: ffff7c7c" hasStroke="0"/>
    <RECT pos="0 300 152 300" fill="solid: ffffd082" hasStroke="0"/>
    <TEXT pos="4 304 292 30" fill="solid: ffff7c7c" hasStroke="0" text="OUTPUT"
          fontname="Futura" fontsize="24.1" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="4 4 292 30" fill="solid: ff88e1f2" hasStroke="0" text="SYNTHESIS"
          fontname="Futura" fontsize="24.1" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="8 168 208 30" fill="solid: a0ffffff" hasStroke="0" text="FEATURE"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="156 4 292 30" fill="solid: ffffd082" hasStroke="0" text="CORPUS"
          fontname="Futura" fontsize="24.1" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="304 570 496 30" fill="solid: ffffffff" hasStroke="0" text="C A T E C O P H O N Y"
          fontname="Futura" fontsize="24.1" kerning="0.309" bold="0" italic="0"
          justification="10" typefaceStyle="Medium"/>
    <TEXT pos="8 100 96 20" fill="solid: a0ffffff" hasStroke="0" text="GRAIN"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="8 116 96 20" fill="solid: a0ffffff" hasStroke="0" text="SIZE"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="80 100 96 20" fill="solid: a0ffffff" hasStroke="0" text="HOP"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="80 116 96 20" fill="solid: a0ffffff" hasStroke="0" text="SIZE"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
    <TEXT pos="8 56 208 30" fill="solid: a0ffffff" hasStroke="0" text="WINDOW TYPE"
          fontname="Futura" fontsize="14.8" kerning="0.147" bold="0" italic="0"
          justification="9" typefaceStyle="Medium"/>
  </BACKGROUND>
  <LABEL name="new label" id="a63405fa3a1ec26f" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 347 132 40" textCol="a0ff7c7c"
         edTextCol="ff000000" edBkgCol="0" labelText="TEMPERATURE" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="14.2" kerning="0.147" bold="0" italic="0" justification="33"
         typefaceStyle="Medium"/>
  <LABEL name="new label" id="71b54d4267d6b770" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 440 116 40" textCol="a0ff7c7c"
         edTextCol="ff000000" edBkgCol="0" labelText="MATCH MAGNITUDE"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Futura" fontsize="10.0" kerning="0.147" bold="0" italic="0"
         justification="34" typefaceStyle="Medium"/>
  <GENERICCOMPONENT name="Loading Bar" id="431af9247eeb453f" memberName="loading_bar"
                    virtualName="" explicitFocusOrder="0" pos="152 0 648 600" class="LoadingBar"
                    params=""/>
  <COMBOBOX name="Feature #3" id="7795db765f687142" memberName="feature_3"
            virtualName="" explicitFocusOrder="0" pos="8 236 136 16" editable="0"
            layout="33" items="None&#10;Dissonance&#10;F0&#10;Inharmonicity&#10;MFCC&#10;Odd:even Harmonic Ratio&#10;Pitch Salience&#10;RMS&#10;Spectral Centroid&#10;Spectral Complexity&#10;Spectral Contrast&#10;Spectral Flatness&#10;Spectral Peaks&#10;Spectral Roll-off&#10;Strong Peak Ratio&#10;Zero Crossing Rate"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Feature #2" id="fb32e96538878318" memberName="feature_2"
            virtualName="" explicitFocusOrder="0" pos="8 212 136 16" editable="0"
            layout="33" items="None&#10;Dissonance&#10;F0&#10;Inharmonicity&#10;MFCC&#10;Odd:even Harmonic Ratio&#10;Pitch Salience&#10;RMS&#10;Spectral Centroid&#10;Spectral Complexity&#10;Spectral Contrast&#10;Spectral Flatness&#10;Spectral Peaks&#10;Spectral Roll-off&#10;Strong Peak Ratio&#10;Zero Crossing Rate"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Feature #1" id="a431f809fa16057a" memberName="feature_1"
            virtualName="" explicitFocusOrder="0" pos="8 188 136 16" editable="0"
            layout="33" items="Dissonance&#10;F0&#10;Inharmonicity&#10;MFCC&#10;Odd:even Harmonic Ratio&#10;Pitch Salience&#10;RMS&#10;Spectral Centroid&#10;Spectral Complexity&#10;Spectral Contrast&#10;Spectral Flatness&#10;Spectral Peaks&#10;Spectral Roll-off&#10;Strong Peak Ratio&#10;Zero Crossing Rate"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="Dry/Wet" id="8e09b0af3f76a8b3" memberName="dry_wet" virtualName=""
          explicitFocusOrder="0" pos="76 528 76 70" bkgcol="ffff7c7c" thumbcol="ff88e1f2"
          trackcol="ffff7c7c" rotarysliderfill="ffff7c7c" rotaryslideroutline="ffff7c7c"
          min="0.0" max="1.0" int="0.01" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="94d65f788b8c0e12" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="76 496 76 48" textCol="a0ff7c7c"
         edTextCol="ff000000" edBkgCol="0" labelText="DRY&#10;WET" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="14.8" kerning="0.147" bold="0" italic="0" justification="36"
         typefaceStyle="Medium"/>
  <TEXTBUTTON name="Analyse Corpus" id="c63f9d0817013fb7" memberName="analyse"
              virtualName="" explicitFocusOrder="0" pos="8 264 136 24" buttonText="Analyse Corpus"
              connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="Grain Size" id="b273822197bc6200" memberName="grain_size"
          virtualName="" explicitFocusOrder="0" pos="8 136 64 16" textboxoutline="a0ffffff"
          min="5.0" max="13.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="2.0"
          needsCallback="1"/>
  <SLIDER name="Hop Size" id="ae573407cf541b0c" memberName="hop_size" virtualName=""
          explicitFocusOrder="0" pos="80 136 64 16" textboxoutline="a0ffffff"
          min="4.0" max="12.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="2.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="Match Grain Magnitude" id="857c68940a60714d" memberName="matchGain"
                virtualName="" explicitFocusOrder="0" pos="120 448 32 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <GENERICCOMPONENT name="Grain View" id="11e7996b86a6f61d" memberName="grainView"
                    virtualName="" explicitFocusOrder="0" pos="152 0 648 600" class="GrainView"
                    params=""/>
  <GENERICCOMPONENT name="Drag And Drop" id="4553fb37de056e3b" memberName="drag_and_drop"
                    virtualName="" explicitFocusOrder="0" pos="152 0 648 600" class="FileDragAndDrop"
                    params="std::move(fileFilter)"/>
  <SLIDER name="Temperature" id="1845712d1d3cf57" memberName="temperature"
          virtualName="" explicitFocusOrder="0" pos="4 368 144 35" bkgcol="ffff7c7c"
          thumbcol="ff88e1f2" trackcol="ffff7c7c" textboxoutline="a0ffffff"
          min="0.0" max="1.0" int="0.001" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <LABEL name="new label" id="6536e42491d21318" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 416 116 40" textCol="a0ff7c7c"
         edTextCol="ff000000" edBkgCol="0" labelText="RELATIVE MATCHING"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Futura" fontsize="10.0" kerning="0.147" bold="0" italic="0"
         justification="34" typefaceStyle="Medium"/>
  <TOGGLEBUTTON name="Relative Matching" id="d38ba4cc76c46fef" memberName="relativeMatching"
                virtualName="" explicitFocusOrder="0" pos="120 424 32 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
  <COMBOBOX name="Feature #1" id="688c048f9bc8a0c" memberName="feature_4"
            virtualName="" explicitFocusOrder="0" pos="8 72 136 16" editable="0"
            layout="33" items="Dissonance&#10;F0&#10;Inharmonicity&#10;MFCC&#10;Odd:even Harmonic Ratio&#10;Pitch Salience&#10;RMS&#10;Spectral Centroid&#10;Spectral Complexity&#10;Spectral Contrast&#10;Spectral Flatness&#10;Spectral Peaks&#10;Spectral Roll-off&#10;Strong Peak Ratio&#10;Zero Crossing Rate"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="Dry/Wet" id="86d0a193389343f2" memberName="dry_wet2" virtualName=""
          explicitFocusOrder="0" pos="0 528 76 70" bkgcol="ffff7c7c" thumbcol="ff88e1f2"
          trackcol="ffff7c7c" rotarysliderfill="ffff7c7c" rotaryslideroutline="ffff7c7c"
          min="0.0" max="1.0" int="0.01" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="b767e3ad4e932aa2" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="0 496 76 48" textCol="a0ff7c7c" edTextCol="ff000000"
         edBkgCol="0" labelText="INPUT&#10;GAIN" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Futura"
         fontsize="14.8" kerning="0.147" bold="0" italic="0" justification="36"
         typefaceStyle="Medium"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

