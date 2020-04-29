/*
  ==============================================================================

    FeatureExtractors.cpp
    Created: 28 Apr 2020 1:30:37pm
    Author:  Ben Hayes

  ==============================================================================
*/

#include "FeatureExtractors.h"

FeatureExtractorChain::FeatureExtractorChain(size_t grainSize)
    : grainSize(grainSize),
      fftExtractor(grainSize)
{
}

void FeatureExtractorChain::addFeatureExtractor(
    Feature feature)
{
    switch (feature)
    {
        case FFT: {
            auto feature = std::make_unique<FFTFeatureExtractor>(grainSize);
            features.add(std::move(feature));
            break;
        }

        case SpectralCentroid: {
            auto feature =
                std::make_unique<SpectralCentroidFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case MaxEnergy:
        break;

        case F0:
        break;
    }
}

void FeatureExtractorChain::resetFeatureExtractors()
{
    features.clear();
}

Array<float> FeatureExtractorChain::process(std::unique_ptr<Grain>& input)
{
    auto fftComputed = false;
    Array<float> fftOutput;
    Array<float> computedFeatures;

    for (auto& feature : features)
    {
        Array<float> computedFeature;
        if (feature->requiresFFT())
        {
            if (!fftComputed)
            {
                fftOutput = fftExtractor.process(input);
            }
            computedFeature = feature->process(fftOutput);
        } else
        {
            computedFeature = feature->process(input);
        }

        for (auto& val : computedFeature)
        {
            computedFeatures.add(val);
        }
    }

    return computedFeatures;
}

FFTFeatureExtractor::FFTFeatureExtractor(size_t grainSize)
    : grainSize(grainSize),
      fftOrder(int(ceil(log2f(grainSize)))),
      fftSize(int(powf(2, fftOrder))),
      fftData(new float[fftSize * 2]),
      fft(fftOrder)
{
}

Array<float> FFTFeatureExtractor::process(
    std::unique_ptr<Grain>& input)
{
    const float* inputBuffer = input->getRawMonoBuffer();
    zeromem(fftData, fftSize * 2 * sizeof(float));
    memcpy(fftData, inputBuffer, grainSize * sizeof(float));

    fft.performFrequencyOnlyForwardTransform(fftData);

    Array<float> fftOut;
    for (int k = 0; k < fftSize / 2 + 1; k++)
    {
        fftOut.add(isnan(fftData[k]) ? 0.0f : fftData[k]);
    }

    return fftOut;
}

Array<float> SpectralCentroidFeatureExtractor::process(Array<float>& fftInput)
{
    auto runningTotal = 0.0f;
    auto weightingTotal = 0.0f;
    for (int k = 0; k < fftInput.size(); k++)
    {
        runningTotal += k * fftInput[k];
        weightingTotal += fftInput[k];
    }
    auto centroid = (runningTotal / weightingTotal) / fftInput.size();
    return isnan(centroid) ? 0.0f : centroid;
}