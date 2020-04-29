/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FeatureExtractors.h"
#include "GrainCorpus.h"

//==============================================================================
/**
*/
class CatecophonyAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    CatecophonyAudioProcessor();
    ~CatecophonyAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================

    void setCorpus(std::unique_ptr<GrainCorpus>);
    GrainCorpus* getCorpus();

    void setFeatureExtractorChain(std::unique_ptr<FeatureExtractorChain>);
    FeatureExtractorChain* getFeatureExtractorChain();

private:
    std::unique_ptr<GrainCorpus> corpus;
    std::unique_ptr<FeatureExtractorChain> featureExtractorChain;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CatecophonyAudioProcessor)
    //==============================================================================
};
