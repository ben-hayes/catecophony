/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    GrainCorpus.cpp
    Description: Implementation file for Grain Corpus
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
    // Constructs a grain corpus with the given parameters from a list of
    // AudioFormatReaders
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
    // shift and scale the input features, and apply some temperature to the
    // sampling
    Array<float> tempFeatures;
    for (int i = 0; i < featuresToCompare.size(); i++)
    {
        auto shiftScale = 
            (featuresToCompare[i] - corpusMean[i]) / corpusStd[i];
        auto eps = normalDist(rng) * temperature;
        tempFeatures.add(shiftScale + eps);
    }

    // find the nearest grain using the search tree
    auto nearestIndex = searchTree->getNearestPoint(tempFeatures);
    lastFeatureMatch = features[nearestIndex];
    matchHistory.add(nearestIndex);
    return grains[nearestIndex];
}

Grain* GrainCorpus::findNearestStep(
    Array<float>& featuresToCompare,
    float temperature)
{
    // Remembers the last matched feature point and takes the equivalent step
    // from there

    // If we're starting again (which we often are)
    if (startChainFromScratch)
    {
        // just find the regular old match
        startChainFromScratch = false;
        return findNearestGrain(featuresToCompare, temperature);
    }

    // but otherwise, create a fictional point that mirrors the desired step
    // and make the match from there
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
    // Given a chain of feature extractors, analyse all the grains in the
    // corpus and store the results internally
    features.clear();

    auto i = 0;
    for (auto& grain : grains)
    {
        features.add(featureExtractors->process(grain));
        i++;
        progressCallback((float)i / grains.size());
    }
    normaliseFeatures();

    searchTree = std::make_unique<KDSearchTree>(features);

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