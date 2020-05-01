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
            auto grainBuffer = std::make_unique<AudioBuffer<float>>(
                reader->numChannels,
                grainLength);
            reader->read(
                grainBuffer.get(),
                0,
                grainLength,
                grainStart,
                true,
                true);

            grains.add(
                std::make_unique<Grain>(std::move(grainBuffer), window));
        }
    }
}

Grain* GrainCorpus::findNearestGrain(Array<float>& featuresToCompare)
{
    auto shortestDistance = 0.0f;
    Grain* nearestGrain;

    for (int i = 0; i < features.size(); i++)
    {
        auto distance = L2Distance(features[i], featuresToCompare);
        if (distance < shortestDistance || i == 0)
        {
            shortestDistance = distance;
            nearestGrain = grains[i];
        }
        
    }
    return nearestGrain;
}

void GrainCorpus::analyse(
    FeatureExtractorChain* featureExtractors)
{
    features.clear();

    for (auto& grain : grains)
    {
        features.add(featureExtractors->process(grain));
    }

    for (auto& f : features)
    {
        for (auto& v : f)
        {
            std::cout<<v<<", ";
        }
        std::cout<<std::endl;
    }

    analysed = true;
}

size_t GrainCorpus::getGrainLength()
{
    return grainLength;
}