/*
  ==============================================================================

    GrainCorpus.h
    Created: 28 Apr 2020 11:38:36am
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
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

    void analyse(
        FeatureExtractorChain* featureExtractors);

    static std::unique_ptr<GrainCorpus> makeCorpusFromAudioFileReaders(
        Array<std::unique_ptr<AudioFormatReader>>&,
        Window::WindowType window = Window::Hann,
        int grainLength = 4096,
        int hopSize = 2048);

    size_t getGrainLength();

private:
    Array<std::unique_ptr<Grain>> grains;
    Array<Array<float>> features;

    size_t grainLength;
    bool analysed = false;
};