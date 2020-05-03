/*
  ==============================================================================

    GrainCorpus.h
    Created: 28 Apr 2020 11:38:36am
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <functional>

#include <JuceHeader.h>

#include "FeatureExtractors.h"
#include "Grains.h"

class GrainCorpus
{
public:
    GrainCorpus(
        Array<std::unique_ptr<AudioFormatReader>>& readers,
        Window::WindowType window = Window::Hann,
        size_t grainLength = 4096,
        size_t hopSize = 2048);
    ~GrainCorpus() {}

    Grain* findNearestGrain(Array<float>& featuresToCompare);
    Grain* findNearestStep(Array<float>& featuresToCompare);
    void resetStepChain();

    void analyse(
        FeatureExtractorChain* featureExtractors,
        std::function<void(float)> progressCallback);
    
    std::unique_ptr<Array<Array<float>>> get3DGrainCoords();
    Array<int> getMatchHistory();

    size_t getGrainLength();

private:
    OwnedArray<Grain> grains;
    Array<Array<float>> features;

    bool startChainFromScratch = true;
    Array<float> lastFeatureMatch;
    Array<float> lastFeatureInput;
    Array<int> matchHistory;

    size_t grainLength;
    bool analysed = false;
};