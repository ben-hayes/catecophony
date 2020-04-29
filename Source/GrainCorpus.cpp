/*
  ==============================================================================

    GrainCorpus.cpp
    Created: 28 Apr 2020 11:38:36am
    Author:  Ben Hayes

  ==============================================================================
*/

#include "GrainCorpus.h"

GrainCorpus::GrainCorpus(
    Array<std::unique_ptr<AudioFormatReader>>& readers,
    Window::WindowType window,
    size_t grainLength,
    size_t hopSize)
    : grainLength(grainLength)
{
    for (auto& reader : readers)
    {
        auto numGrains = 1 + static_cast<int>(
            floorf((reader->lengthInSamples - grainLength) / hopSize));
        
        for (int i = 0; i < numGrains; i++)
        {
            auto grainStart = hopSize * i;
            auto* grainBuffer = new AudioBuffer<float>(
                reader->numChannels,
                grainLength);
            reader->read(grainBuffer, 0, grainLength, grainStart, true, true);

            grains.add(std::make_unique<Grain>(
                std::unique_ptr<AudioBuffer<float>>(grainBuffer),
                window
            ));
        }
    }
}

void GrainCorpus::analyse(
    FeatureExtractorChain* featureExtractors)
{
    if (analysed) features.clear();

    for (auto& grain : grains)
    {
        features.add(featureExtractors->process(grain));
    }

    analysed = true;
}

size_t GrainCorpus::getGrainLength()
{
    return grainLength;
}