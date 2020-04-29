/*
  ==============================================================================

    FeatureExtractors.h
    Created: 28 Apr 2020 1:30:37pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include<JuceHeader.h>

#include "Grains.h"

enum Feature
{
    FFT = 0,
    SpectralCentroid,
    MaxEnergy,
    F0
};

class FeatureExtractor
{
public:
    virtual ~FeatureExtractor() {};

    virtual Array<float> process(
        std::unique_ptr<Grain>& input) = 0;
    virtual Array<float> process(Array<float>& fftInput) { return {}; };
    virtual bool requiresFFT() = 0;
};

class FFTBasedFeatureExtractor : public FeatureExtractor
{
public:
    virtual ~FFTBasedFeatureExtractor() {};
    Array<float> process(
        std::unique_ptr<Grain>& input) override { return {}; };
    virtual Array<float> process(Array<float>& fftInput) override = 0;

    bool requiresFFT() override { return true; };
};

class FFTFeatureExtractor : public FeatureExtractor
{
public:
    FFTFeatureExtractor(size_t grainSize);

    Array<float> process(
        std::unique_ptr<Grain>& input) override;
    bool requiresFFT() override { return false; };

private:
    size_t grainSize;
    size_t fftOrder;
    size_t fftSize;
    float* fftData;

    dsp::FFT fft;
};

class FeatureExtractorChain : public FeatureExtractor
{
public:
    FeatureExtractorChain(size_t grainSize);

    void addFeatureExtractor(Feature feature);
    void resetFeatureExtractors();

    Array<float> process(
        std::unique_ptr<Grain>& input) override;
    bool requiresFFT() override { return false; };

private:
    size_t grainSize;
    FFTFeatureExtractor fftExtractor;
    Array<std::unique_ptr<FeatureExtractor>> features;
};

class SpectralCentroidFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    Array<float> process(Array<float>& fftInput) override;
};