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
    auto matchedIndex = 0;

    for (int i = 0; i < features.size(); i++)
    {
        auto distance = L2Distance(features[i], featuresToCompare);
        if (distance < shortestDistance || i == 0)
        {
            shortestDistance = distance;
            nearestGrain = grains[i];
            matchedIndex = i;
        }
        
    }
    matchHistory.add(matchedIndex);
    return nearestGrain;
}

Grain* GrainCorpus::findNearestStep(Array<float>& featuresToCompare)
{
    if (startChainFromScratch)
    {
        startChainFromScratch = false;
        return findNearestGrain(featuresToCompare);
    }

    Array<float> imaginaryFeatures;
    for (int i = 0; i < featuresToCompare.size(); i++)
    {
        imaginaryFeatures.add(
            lastFeatureMatch[i]
            + featuresToCompare[i]
            - lastFeatureInput[i]);
    }

    return findNearestGrain(imaginaryFeatures);
}

void GrainCorpus::resetStepChain()
{
    startChainFromScratch = true;
}

void GrainCorpus::analyse(
    FeatureExtractorChain* featureExtractors,
    std::function<void(float)> progressCallback)
{
    features.clear();

    auto i = 0;
    for (auto& grain : grains)
    {
        features.add(featureExtractors->process(grain));
        i++;
        progressCallback((float)i / grains.size());
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

std::unique_ptr<Array<Array<float>>> GrainCorpus::get3DGrainCoords()
{
    auto pcaGrains = pcaReduce(features);
    return std::move(pcaGrains);
}

Array<int> GrainCorpus::getMatchHistory()
{
    auto mh = matchHistory;
    matchHistory.clear();
    return mh;
}

size_t GrainCorpus::getGrainLength()
{
    return grainLength;
}