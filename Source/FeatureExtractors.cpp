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

        case F0: {
            auto feature =
                std::make_unique<PitchFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case SpectralFlatness: {
            auto feature =
                std::make_unique<SpectralFlatnessFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case SpectralComplexity: {
            auto feature =
                std::make_unique<SpectralComplexityFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case SpectralRollOff: {
            auto feature =
                std::make_unique<SpectralRollOffFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case SpectralContrast: {
            auto feature =
                std::make_unique<SpectralContrastFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case SpectralPeaks: {
            auto feature =
                std::make_unique<SpectralPeaksFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case StrongPeak: {
            auto feature =
                std::make_unique<StrongPeakFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case ZeroCrossingRate: {
            auto feature =
                std::make_unique<ZeroCrossingFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }

        case RMS: {
            auto feature =
                std::make_unique<RMSFeatureExtractor>();
            features.add(std::move(feature));
            break;
        }
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
            allFeatures.push_back(isnan(value) || isinf(value) ? 0.0f : value);
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

SpectralFlatnessFeatureExtractor::SpectralFlatnessFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    flatness.reset(algorithmFactory.create(
        "Flatness"
    ));
}

std::vector<e::Real> SpectralFlatnessFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    e::Real output;
    flatness->input("array").set(fftInput);
    flatness->output("flatness").set(output);
    flatness->compute();

    return std::vector<e::Real>{output};
}

SpectralComplexityFeatureExtractor::SpectralComplexityFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    complexity.reset(algorithmFactory.create(
        "SpectralComplexity"
    ));
}

std::vector<e::Real> SpectralComplexityFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    e::Real output;
    complexity->input("spectrum").set(fftInput);
    complexity->output("spectralComplexity").set(output);
    complexity->compute();

    return std::vector<e::Real>{output / (float)(fftInput.size())};
}

SpectralRollOffFeatureExtractor::SpectralRollOffFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    rollOff.reset(algorithmFactory.create(
        "RollOff"
    ));
}

std::vector<e::Real> SpectralRollOffFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    e::Real output;
    rollOff->input("spectrum").set(fftInput);
    rollOff->output("rollOff").set(output);
    rollOff->compute();

    return std::vector<e::Real>{output / 22050.0f};
}

SpectralContrastFeatureExtractor::SpectralContrastFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    contrast.reset(algorithmFactory.create(
        "SpectralContrast"
    ));
}

std::vector<e::Real> SpectralContrastFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    std::vector<e::Real> spectralContrast;
    std::vector<e::Real> spectralValley;

    e::ParameterMap pm;
    e::Parameter numberBands(4);
    e::Parameter frameSize(int((fftInput.size() - 1) * 2));
    pm.add("numberBands", numberBands);
    pm.add("frameSize", frameSize);
    contrast->configure(pm);

    contrast->input("spectrum").set(fftInput);
    contrast->output("spectralContrast").set(spectralContrast);
    contrast->output("spectralValley").set(spectralValley);
    contrast->compute();

    std::vector<e::Real> out;
    for (auto val : spectralContrast)
    {
        out.push_back(expf(val));
    }

    return out;
}

SpectralPeaksFeatureExtractor::SpectralPeaksFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    peaks.reset(algorithmFactory.create(
        "SpectralPeaks",
        "maxPeaks", numPeaks,
        "orderBy", "magnitude"
    ));
}

std::vector<e::Real> SpectralPeaksFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    auto sum = 0.0f;
    for (auto& val : fftInput) sum += val;
    if (sum <= 0.0001)
    {
        std::cout<<"Nothing!"<<std::endl;
    }
    std::vector<e::Real> freqs;
    std::vector<e::Real> mags;
    peaks->input("spectrum").set(fftInput);
    peaks->output("frequencies").set(freqs);
    peaks->output("magnitudes").set(mags);
    peaks->compute();

    std::vector<e::Real> out;
    for (auto& val : freqs)
    {
        out.push_back(val / 22050.0f);
    }
    for (int i = 0; i < numPeaks - freqs.size(); i++)
    {
        out.push_back(0.0f);
    }

    return out;
}

StrongPeakFeatureExtractor::StrongPeakFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    strongPeak.reset(algorithmFactory.create(
        "StrongPeak"
    ));
}

std::vector<e::Real> StrongPeakFeatureExtractor::process(
    std::vector<e::Real>& fftInput)
{
    e::Real strongPeakValue;
    strongPeak->input("spectrum").set(fftInput);
    strongPeak->output("strongPeak").set(strongPeakValue);
    strongPeak->compute();

    return std::vector<e::Real>{strongPeakValue};
}

ZeroCrossingFeatureExtractor::ZeroCrossingFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    zeroCrossing.reset(algorithmFactory.create(
        "ZeroCrossingRate"
    ));
}

std::vector<e::Real> ZeroCrossingFeatureExtractor::process(
    std::vector<e::Real>& input)
{
    e::Real zeroCrossingRate;
    zeroCrossing->input("signal").set(input);
    zeroCrossing->output("zeroCrossingRate").set(zeroCrossingRate);
    zeroCrossing->compute();

    return std::vector<e::Real>{zeroCrossingRate};
}

RMSFeatureExtractor::RMSFeatureExtractor()
    : algorithmFactory(es::AlgorithmFactory::instance())
{
    rms.reset(algorithmFactory.create(
        "RMS"
    ));
}

std::vector<e::Real> RMSFeatureExtractor::process(std::vector<e::Real>& input)
{
    e::Real rmsVal;
    rms->input("array").set(input);
    rms->output("rms").set(rmsVal);
    rms->compute();

    return std::vector<e::Real>{rmsVal};
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
    else if (extractorName == "Spectral Flatness")
        return SpectralFlatness;
    else if (extractorName == "Spectral Complexity")
        return SpectralComplexity;
    else if (extractorName == "Spectral Roll-off")
        return SpectralRollOff;
    else if (extractorName == "Spectral Contrast")
        return SpectralContrast;
    else if (extractorName == "Spectral Peaks")
        return SpectralPeaks;
    else if (extractorName == "Strong Peak Ratio")
        return StrongPeak;
    else if (extractorName == "Zero Crossing Rate")
        return ZeroCrossingRate;
    else if (extractorName == "RMS")
        return RMS;
    else
        throw new UnknownExtractorException();
}

std::unique_ptr<Array<Array<float>>> pcaReduce(
    Array<Array<float>>& grains,
    int nDims)
{
    es::AlgorithmFactory& af = es::AlgorithmFactory::instance();
    auto pca = af.create(
        "PCA",
        "dimensions", nDims,
        "namespaceIn", "pca.in",
        "namespaceOut", "pca.out"
    );

    e::Pool pool;
    for (auto& grain : grains)
    {
        std::vector<e::Real> realFeatures;
        for (auto& feature : grain)
        {
            realFeatures.push_back(feature);
        }

        pool.add("pca.in", realFeatures);
    }

    pca->input("poolIn").set(pool);
    pca->output("poolOut").set(pool);
    try
    {
        pca->compute();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    const std::vector<std::vector<e::Real>>& pcaOut =
        pool.value<std::vector<std::vector<e::Real>>>("pca.out");
    
    auto out = std::make_unique<Array<Array<float>>>();
    for (auto& grain : pcaOut)
    {
        Array<float> floatFeatures;
        for (auto& feature : grain)
        {
            floatFeatures.add(feature);
        }

        out->add(floatFeatures);
    }
    return std::move(out);
}

Array<float> getFeatureMeans(Array<Array<float>>& features)
{
    Array<float> runningMeans;
    for (int i = 0; i < features[0].size(); i++) runningMeans.add(0.0f);

    for (auto& feature : features)
    {
        for (int i = 0; i < feature.size(); i++)
        {
            runningMeans.setUnchecked(i, runningMeans[i] + feature[i]);
        }
    }

    for (int i = 0; i < features[0].size(); i++)
        runningMeans.setUnchecked(i, runningMeans[i] / features.size());
    
    return runningMeans;
}

Array<float> getFeatureStds(
    Array<Array<float>>& features,
    Array<float>& means)
{
    Array<float> runningStds;
    for (int i = 0; i < features[0].size(); i++) runningStds.add(0.0f);

    for (auto& feature : features)
    {
        for (int i = 0; i < feature.size(); i++)
        {
            runningStds.setUnchecked(
                i,
                runningStds[i] + powf(feature[i] - means[i], 2.0f));
        }
    }

    for (int i = 0; i < features[0].size(); i++)
    {
        runningStds.setUnchecked(i, runningStds[i] / features.size());
        runningStds.setUnchecked(i, sqrtf(runningStds[i]));
    }
    
    return runningStds;
}