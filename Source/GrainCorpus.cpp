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
    : grainLength(grainLength),
      normalDist(0.0f, 1.0f)
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
            auto grain =
                std::make_unique<Grain>(std::move(grainBuffer), window);
            if (!grain->isSilent())
                grains.add(std::move(grain));
        }
    }
}

Grain* GrainCorpus::findNearestGrain(
    Array<float>& featuresToCompare,
    float temperature)
{
    auto shortestDistance = 0.0f;
    Grain* nearestGrain;
    auto matchedIndex = 0;

    Array<float> tempFeatures;
    for (int i = 0; i < featuresToCompare.size(); i++)
    {
        auto shiftScale = 
            (featuresToCompare[i] - corpusMean[i]) / corpusStd[i];
        auto eps = normalDist(rng) * temperature;
        tempFeatures.add(shiftScale + eps);
    }

    for (int i = 0; i < features.size(); i++)
    {
        auto distance = L2Distance(features[i], tempFeatures);
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

Grain* GrainCorpus::findNearestStep(
    Array<float>& featuresToCompare,
    float temperature)
{
    if (startChainFromScratch)
    {
        startChainFromScratch = false;
        return findNearestGrain(featuresToCompare, temperature);
    }

    Array<float> imaginaryFeatures;
    for (int i = 0; i < featuresToCompare.size(); i++)
    {
        imaginaryFeatures.add(
            lastFeatureMatch[i]
            + featuresToCompare[i]
            - lastFeatureInput[i]);
    }

    return findNearestGrain(imaginaryFeatures, temperature);
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
    normaliseFeatures();

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

void GrainCorpus::normaliseFeatures()
{
    corpusMean = getFeatureMeans(features);
    corpusStd = getFeatureStds(features, corpusMean);

    for (auto& feature : features)
    {
        for (int i = 0; i < feature.size(); i++)
        {
            feature.setUnchecked(i, feature[i] - corpusMean[i]);
            feature.setUnchecked(i, feature[i] / corpusStd[i]);
        }
    }
}