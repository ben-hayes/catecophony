/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    AnalysisWorker.h
    Description: Worker thread for analysing corpus - header
  ==============================================================================
*/

#pragma once
#include <functional>

#include <JuceHeader.h>

#include "GrainCorpus.h"
#include "PluginProcessor.h"

class CatecophonyAudioProcessor;
class AnalysisWorker : public Thread
{
public:
    AnalysisWorker(
        CatecophonyAudioProcessor& processor,
        const StringArray files,
        int grainSize,
        int hopSize,
        Array<Feature> features,
        Window::WindowType window,
        std::function<void()> finishedCallback);
    AnalysisWorker(
        CatecophonyAudioProcessor& processor,
        int grainSize,
        int hopSize,
        Array<Feature> features,
        Window::WindowType window,
        std::function<void()> finishedCallback);
    ~AnalysisWorker();

    void run() override;
    float getProgress() {return progress;}

private:
    CatecophonyAudioProcessor& processor;
    const StringArray files;
    Window::WindowType window;
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