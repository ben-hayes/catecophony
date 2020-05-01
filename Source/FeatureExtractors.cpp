/*
  ==============================================================================

    FeatureExtractors.cpp
    Created: 28 Apr 2020 1:30:37pm
    Author:  Ben Hayes

  ==============================================================================
*/
#include "FeatureExtractors.h"

bool FactoryInitialiser::alreadyInitialised = false;

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

        case MFCC: {
            auto feature =
                std::make_unique<MFCCFeatureExtractor>(grainSize);
            features.add(std::move(feature));
            break;
        }

        case F0:
            auto feature =
                std::make_unique<PitchFeatureExtractor>();
            features.add(std::move(feature));
        break;
    }
}

void FeatureExtractorChain::resetFeatureExtractors()
{
    features.clear();
}

std::vector<e::Real> FeatureExtractorChain::process(
    std::vector<e::Real>& input)
{
    auto fftComputed = false;
    std::vector<e::Real> magSpectrum;
    std::vector<e::Real> allFeatures;

    for (auto& feature : features)
    {
        std::vector<e::Real> computedFeatures;
        if (feature->requiresFFT())
        {
            if (!fftComputed)
            {
                magSpectrum = fftExtractor.process(input);
            }
            computedFeatures = feature->process(magSpectrum);
        } else
        {
            computedFeatures = feature->process(input);
        }

        for (auto value : computedFeatures)
        {
            allFeatures.push_back(isnan(value) ? 0.0f : value);
        }
    }

    return allFeatures;
}

Array<float> FeatureExtractorChain::process(Grain* input)
{
    auto* inputBuffer = input->getRawMonoBuffer();
    std::vector<e::Real> inputSignal;
    std::vector<e::Real> computedFeatures;
    Array<float> outputFeatures;

    for (int n = 0; n < input->getBufferLength(); n++)
    {
        inputSignal.push_back(inputBuffer[n]);
    }
    
    computedFeatures = this->process(inputSignal);
    for (auto value : computedFeatures)
    {
        outputFeatures.add(value);
    }

    return outputFeatures;

}

FFTFeatureExtractor::FFTFeatureExtractor(size_t grainSize)
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    auto fftOrder = (int)ceil(log2f(grainSize));
    fftSize = (int)powf(2, fftOrder);

    fft.reset(algorithmFactory.create(
        "Spectrum",
        "size", fftSize));
}

std::vector<e::Real> FFTFeatureExtractor::process(std::vector<e::Real>& input)
{
    std::vector<e::Real> output;

    fft->input("frame").set(input);
    fft->output("spectrum").set(output);
    fft->compute();

    return output;
}

SpectralCentroidFeatureExtractor::SpectralCentroidFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    centroid.reset(algorithmFactory.create("Centroid"));
}

std::vector<e::Real> SpectralCentroidFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    e::Real centroidValue;
    std::vector<e::Real> output;

    centroid->input("array").set(fftInput);
    centroid->output("centroid").set(centroidValue);
    centroid->compute();
    output.push_back(centroidValue);
    return output;
}

MFCCFeatureExtractor::MFCCFeatureExtractor(size_t grainSize, int numMFCCs)
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    auto fftOrder = (int)ceil(log2f(grainSize));
    auto fftSize = (int)powf(2, fftOrder);
    inSize = fftSize / 2 + 1;

    mfcc.reset(algorithmFactory.create(
        "MFCC",
        "inputSize", inSize,
        "numberCoefficients", numMFCCs
    ));
}

std::vector<e::Real> MFCCFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    std::vector<e::Real> coeffs;
    std::vector<e::Real> bands;

    mfcc->input("spectrum").set(fftInput);
    mfcc->output("mfcc").set(coeffs);
    mfcc->output("bands").set(bands);
    mfcc->compute();

    for (auto& coeff : coeffs)
    {
        coeff /= inSize;
    }

    return coeffs;
}

PitchFeatureExtractor::PitchFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    yin.reset(algorithmFactory.create(
        "PitchYin"
    ));
}

std::vector<e::Real> PitchFeatureExtractor::process(
    std::vector<e::Real>& input)
{
    e::Real pitch;
    e::Real confidence;
    yin->input("signal").set(input);
    yin->output("pitch").set(pitch);
    yin->output("pitchConfidence").set(confidence);
    yin->compute();

    std::vector<e::Real> output{pitch / 22050.0f, confidence};
    return output;
}

float L2Distance(Array<float> a, Array<float> b)
{
    if (a.size() != b.size()) throw new MismatchingArrayLengthException();

    auto runningSum = 0.0f;
    for (int i = 0; i < a.size(); i++)
    {
        runningSum += powf(a[i] - b[i], 2.0f);
    }

    return sqrtf(runningSum);
}

Feature getExtractorByString(String extractorName)
{
    if (extractorName == "Spectral Centroid")
        return SpectralCentroid;
    else if (extractorName == "MFCC")
        return MFCC;
    else if (extractorName == "F0")
        return F0;
    else
        return SpectralCentroid;
        // throw new UnknownExtractorException();
}