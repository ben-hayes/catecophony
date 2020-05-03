/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AnalysisWorker.h"
#include "FeatureExtractors.h"
#include "Grains.h"
#include "GrainCorpus.h"

class AnalysisWorker;
enum class ProcessorState
{
    NoCorpus = 0,
    LoadingFiles,
    Analysing,
    Ready
};

enum class BufferState
{
    NotInUse = 0,
    InUse
};

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
    void processBlockBypassed (AudioBuffer<float>&, MidiBuffer&) override {}

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

    void setGrainAndHopSize(size_t grainSize, size_t hopSize);

    ProcessorState getState();
    BufferState getBufferState();

    void setState(ProcessorState state);
    void setCorpus(std::unique_ptr<GrainCorpus>);
    GrainCorpus* getCorpus();

    void setFeatureExtractorChain(std::unique_ptr<FeatureExtractorChain>);
    FeatureExtractorChain* getFeatureExtractorChain();

    void initialiseCorpusFromFilenames(
        const StringArray& files,
        std::function<void()> finishedCallback);
    void analyseCorpus(std::function<void()> finishedCallback);

    float getWorkerProgress();

private:
    AudioProcessorValueTreeState params;
    ValueTree corpusFiles;

    std::atomic<float>* dryWet;
    ProcessorState state = ProcessorState::NoCorpus;
    BufferState bufState = BufferState::NotInUse;

    std::unique_ptr<AnalysisWorker> worker;

    size_t grainSize = 4096;
    size_t hopSize = 2048;
    int hopCounter = 0;

    float** grainBuffer;
    float** nextGrain;
    Grain workingGrain;

    float** outputBuffer;
    int outputBufferReadPointer = 0;
    int outputBufferWritePointer = 0;

    int grainBufferWritePointer;

    std::unique_ptr<GrainCorpus> corpus;
    std::unique_ptr<FeatureExtractorChain> featureExtractorChain;

    void initialiseBuffers();
    int getSelectedGrainSize();
    int getSelectedHopSize();
    Array<Feature> getSelectedFeatures();

    void addFilenamesToValueTree(const StringArray& files);
    void reloadCorpusFromValueTree();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CatecophonyAudioProcessor)
    //==============================================================================
};
