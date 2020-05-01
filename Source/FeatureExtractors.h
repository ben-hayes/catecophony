/*
  ==============================================================================

    FeatureExtractors.h
    Created: 28 Apr 2020 1:30:37pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <exception>

#include <essentia/algorithmfactory.h>
#include<JuceHeader.h>

#include "Grains.h"

namespace e = essentia;
namespace es = essentia::standard;

enum Feature
{
    FFT = 0,
    SpectralCentroid,
    MFCC,
    F0,
    SpectralFlatness,
    SpectralComplexity,
    SpectralRollOff,
    SpectralContrast,
    SpectralPeaks,
    StrongPeak,
    ZeroCrossingRate,
    MaxMagFreq,
    RMS
};

struct FactoryInitialiser
{
    static bool alreadyInitialised;
    FactoryInitialiser()
    {
        if (!alreadyInitialised)
        {
            e::init();
            alreadyInitialised = true;
        }
    }
};

class FeatureExtractor
{
public:
    virtual ~FeatureExtractor() {};

    virtual std::vector<e::Real> process(std::vector<e::Real>& input) = 0;

    virtual bool requiresFFT() { return false; };

private:
    FactoryInitialiser fi;
};

class FFTBasedFeatureExtractor : public FeatureExtractor
{
public:
    virtual ~FFTBasedFeatureExtractor() {};

    virtual std::vector<e::Real> process(
        std::vector<e::Real>& fftInput) override = 0;

    bool requiresFFT() override { return true; };
};

class FFTFeatureExtractor : public FeatureExtractor
{
public:
    FFTFeatureExtractor(size_t grainSize);

    std::vector<e::Real> process(std::vector<e::Real>& input) override;
    bool requiresFFT() override { return false; };

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> fft;

    int fftSize;
};

class FeatureExtractorChain : public FeatureExtractor
{
public:
    FeatureExtractorChain(size_t grainSize);

    void addFeatureExtractor(Feature feature);
    void resetFeatureExtractors();

    std::vector<e::Real> process(std::vector<e::Real>& input) override;
    Array<float> process(Grain* input);
    bool requiresFFT() override { return false; };

private:
    size_t grainSize;
    FFTFeatureExtractor fftExtractor;
    Array<std::unique_ptr<FeatureExtractor>> features;
};

class SpectralCentroidFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralCentroidFeatureExtractor();

    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> centroid;
};

class MFCCFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    MFCCFeatureExtractor(size_t grainSize, int numMFCCs = 6);

    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> mfcc;

    int inSize;
};

class PitchFeatureExtractor : public FeatureExtractor
{
public:
    PitchFeatureExtractor();

    std::vector<e::Real> process(std::vector<e::Real>& input) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> yin;
};

class SpectralFlatnessFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralFlatnessFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> flatness;
};

class SpectralComplexityFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralComplexityFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> complexity;
};

class SpectralRollOffFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralRollOffFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> rollOff;
};

class SpectralContrastFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralContrastFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> contrast;
};

class SpectralPeaksFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    SpectralPeaksFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    int numPeaks = 4;

    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> peaks;
};

class StrongPeakFeatureExtractor : public FFTBasedFeatureExtractor
{
public:
    StrongPeakFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& fftInput) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> strongPeak;
};

class ZeroCrossingFeatureExtractor : public FeatureExtractor
{
public:
    ZeroCrossingFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& input) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> zeroCrossing;
};

class RMSFeatureExtractor : public FeatureExtractor
{
public:
    RMSFeatureExtractor();
    std::vector<e::Real> process(std::vector<e::Real>& input) override;

private:
    es::AlgorithmFactory& algorithmFactory;
    std::unique_ptr<es::Algorithm> rms;
};

class MismatchingArrayLengthException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Input array lengths do not match";
    }
};

float L2Distance(Array<float> a, Array<float> b);

class UnknownExtractorException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Unknown extractor string given";
    }
};

Feature getExtractorByString(String extractorName);