/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AnalysisWorker.h"
#include "GrainCorpus.h"
#include "MainGui.h"
#include "PluginLookAndFeel.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CatecophonyAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    CatecophonyAudioProcessorEditor (
      CatecophonyAudioProcessor&,
      AudioProcessorValueTreeState&);
    ~CatecophonyAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CatecophonyAudioProcessor& processor;
    AudioProcessorValueTreeState& params;

    std::unique_ptr<AnalysisWorker> worker;

    std::unique_ptr<MainGui> gui;
    PluginLookAndFeel skin;

    AudioFormatManager formatManager;

    void initialiseCorpusFromFilenames(const StringArray& files);
    void analyseCorpus();

    int getSelectedGrainSize();
    int getSelectedHopSize();
    Array<Feature> getSelectedFeatures();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CatecophonyAudioProcessorEditor)
};
