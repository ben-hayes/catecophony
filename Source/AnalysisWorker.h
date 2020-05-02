/*
  ==============================================================================

    AnalysisWorker.h
    Created: 1 May 2020 6:04:15pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <functional>

#include <JuceHeader.h>

#include "GrainCorpus.h"
#include "PluginProcessor.h"

class AnalysisWorker : public Thread
{
public:
    AnalysisWorker(
        CatecophonyAudioProcessor& processor,
        const StringArray files,
        int grainSize,
        int hopSize,
        Array<Feature> features,
        std::function<void()> finishedCallback);
    AnalysisWorker(
        CatecophonyAudioProcessor& processor,
        int grainSize,
        int hopSize,
        Array<Feature> features,
        std::function<void()> finishedCallback);
    ~AnalysisWorker();

    void run() override;
    float getProgress() {return progress;}

private:
    CatecophonyAudioProcessor& processor;
    const StringArray files;
    int grainSize;
    int hopSize;
    Array<Feature> features;
    std::function<void()> finishedCallback;

    float progress;

    bool loadFiles;

    AudioFormatManager formatManager;

    void loadCorpusFiles();
    void analyseCorpus();
};