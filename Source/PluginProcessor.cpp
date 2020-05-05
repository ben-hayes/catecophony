/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    PluginProcessor.cpp
    Description: Implementation file for Catecophony's JUCE plugin processor
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CatecophonyAudioProcessor::CatecophonyAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       ),
        grainBufferWritePointer(0),
        outputBufferReadPointer(0),
        params(*this, nullptr, "Catecophony", {
                std::make_unique<AudioParameterFloat>(
                    "drywet", "Dry/Wet",
                    0.0f, 1.0f, 1.0f),
                std::make_unique<AudioParameterFloat>(
                    "lpfCutoff", "LPF Cutoff",
                    NormalisableRange<float>{0.001f, 1.0f, 0.001f, 0.25f},
                    1.0f),
                std::make_unique<AudioParameterFloat>(
                    "temperature", "Temperature",
                    0.0f, 2.0f, 0.0f),
                std::make_unique<AudioParameterInt>(
                    "grainSize", "Grain Size",
                    5, 13, 11, String(),
                    [](double val, int) {
                        return String((int)powf(2.0f, val + 1));
                    }),
                std::make_unique<AudioParameterInt>(
                    "hopSize", "Hop Length",
                    4, 12, 10, String(),
                    [](double val, int) {
                        return String((int)powf(2.0f, val + 1));
                    }),
                std::make_unique<AudioParameterBool>(
                    "matchGain", "Match Grain Magnitude",
                    true),
                std::make_unique<AudioParameterBool>(
                    "relativeMatching", "Relative Matching",
                    true),
                std::make_unique<AudioParameterChoice>(
                    "window_type", "Window Type",
                    StringArray({
                        "Rectangular",
                        "Triangular",
                        "Hann",
                        "Hamming"
                    }),
                    2),
                std::make_unique<AudioParameterChoice>(
                    "feature_1", "Feature #1",
                    StringArray({
                        "F0",
                        "MFCC",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Contrast",
                        "Spectral Flatness",
                        "Spectral Peaks",
                        "Spectral Roll-off",
                        "Strong Peak Ratio",
                        "Zero Crossing Rate"
                    }),
                    6),
                std::make_unique<AudioParameterChoice>(
                    "feature_2", "Feature #2",
                    StringArray({
                        "None",
                        "F0",
                        "MFCC",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Contrast",
                        "Spectral Flatness",
                        "Spectral Peaks",
                        "Spectral Roll-off",
                        "Strong Peak Ratio",
                        "Zero Crossing Rate"
                    }),
                    4), 
                std::make_unique<AudioParameterChoice>(
                    "feature_3", "Feature #3",
                    StringArray({
                        "None",
                        "F0",
                        "MFCC",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Contrast",
                        "Spectral Flatness",
                        "Spectral Peaks",
                        "Spectral Roll-off",
                        "Strong Peak Ratio",
                        "Zero Crossing Rate"
                    }),
                    9)
        }),
        corpusFiles("CorpusFiles", {})
{
    dryWet = params.getRawParameterValue("drywet");
    lpfCutoff = params.getRawParameterValue("lpfCutoff");
    temperature = params.getRawParameterValue("temperature");
}

CatecophonyAudioProcessor::~CatecophonyAudioProcessor()
{
}

//==============================================================================
const String CatecophonyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CatecophonyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CatecophonyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CatecophonyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CatecophonyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CatecophonyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CatecophonyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CatecophonyAudioProcessor::setCurrentProgram (int index)
{
}

const String CatecophonyAudioProcessor::getProgramName (int index)
{
    return {};
}

void CatecophonyAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CatecophonyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setLatencySamples(grainSize);
    initialiseBuffers();
}

void CatecophonyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CatecophonyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CatecophonyAudioProcessor::processBlock (
    AudioBuffer<float>& buffer,
    MidiBuffer& midiMessages)
{
    // Auto generated JUCE boilerplate for preventing extra channel nastiness:
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    

    // If we're not ready to process, just return. JUCE will just pass the
    // audio through untouched.
    if (state != ProcessorState::Ready) return;

    // Lock the buffers
    bufState = BufferState::InUse;

    // If we've changed the filter control
    if (*lpfCutoff != lastCutoff)
    {
        // Make a new set of Butterworth coefficients and store them in the
        // filter
        auto coeffs = IIRCoefficients::makeLowPass(
            getSampleRate(),
            getSampleRate() * 0.5f * *lpfCutoff);
        lpf[0].setCoefficients(coeffs);
        lpf[1].setCoefficients(coeffs);
        lastCutoff = *lpfCutoff;
    }

    // Now for our main processing loop
    for (int n = 0; n < buffer.getNumSamples(); n++)
    {
        // First we populate our input circular buffer with the samples
        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            grainBuffer[channel][grainBufferWritePointer] =
                buffer.getSample(channel, n);
        }

        // Then move the write pointer round the buffer, wrapping if necessary
        grainBufferWritePointer += 1;
        if (grainBufferWritePointer >= grainSize)
        {
            grainBufferWritePointer = 0;
        }

        hopCounter += 1;
        if (hopCounter >= hopSize)
        {
            // If we have enough for a new grain ...
            hopCounter = 0;
            for (int i = 0; i < grainSize; i++)
            {
                for (int c = 0; c < 2; c++)
                {
                    // Copy the grain to our working grain buffer
                    nextGrain[c][i] = grainBuffer[c][
                        (grainBufferWritePointer + i) % grainSize];
                }
            }

            // And initialise our instance of the Grain class
            workingGrain.init(nextGrain, 2, grainSize, window);

            // No point wasting energy on the next bit if the grain is silent
            if (!workingGrain.isSilent())
            {
                // get parameter values from value tree
                auto matchGain = dynamic_cast<AudioParameterBool*>(
                    params.getParameter("matchGain"))->get();
                auto relativeMatching = dynamic_cast<AudioParameterBool*>(
                    params.getParameter("relativeMatching"))->get();

                // extract features from input grain
                auto features = featureExtractorChain->process(&workingGrain);
                // and find the nearest matching grain, using the method set
                // by the user
                auto* nearestGrain = relativeMatching
                    ? corpus->findNearestStep(features, *temperature)
                    : corpus->findNearestGrain(features, *temperature);
                auto** rawGrainBuffer = nearestGrain->getRawBuffer();
                auto gainScale = matchGain
                    ? workingGrain.getMagnitude() / nearestGrain->getMagnitude()
                    : 1.0f;

                for (int i = 0; i < grainSize; i++)
                {
                    for (int c = 0; c < 2; c++)
                    {
                        // Now overlap-add it to the output circular buffer
                        outputBuffer[c][(outputBufferWritePointer + i)
                                        % (grainSize + hopSize)]
                            += rawGrainBuffer[c][i] * gainScale;
                    }
                }
            } else
            {
                // If we've got a zero sample, it's a good opportunity to reset
                // our path matching
                corpus->resetStepChain();
            }

            // Increment the write pointer, a hop at a time (so we overlap)
            outputBufferWritePointer += hopSize;
            if (outputBufferWritePointer >= grainSize + hopSize)
            {
                outputBufferWritePointer =
                    outputBufferWritePointer % (grainSize + hopSize);
            }
        }

        // Now our output loop -- fairly self explanatory
        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            auto inSample = buffer.getSample(channel, n);
            auto wetSample = *lpfCutoff != 1.0f
                ? lpf[channel].processSingleSampleRaw(
                    outputBuffer[channel][outputBufferReadPointer])
                : outputBuffer[channel][outputBufferReadPointer];
            auto out = *dryWet * wetSample + (1.0f - *dryWet) * inSample;
            buffer.setSample(
                channel,
                n,
                out);
            // This is important though -- we zero the output buffer after
            // reading from it:
            outputBuffer[channel][outputBufferReadPointer] = 0.0f;
        }

        outputBufferReadPointer += 1;
        if (outputBufferReadPointer >= (grainSize + hopSize))
        {
            outputBufferReadPointer = 0;
        }
    }
    // Clear the lock
    bufState = BufferState::NotInUse;
}

//==============================================================================
bool CatecophonyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CatecophonyAudioProcessor::createEditor()
{
    return new CatecophonyAudioProcessorEditor (*this, params);
}

//==============================================================================
void CatecophonyAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Store plugin state using a value tree
    ValueTree totalState("CatecophonyState");
    auto paramState = params.copyState();
    totalState.appendChild(paramState, nullptr);
    totalState.appendChild(corpusFiles, nullptr);
    std::unique_ptr<XmlElement> stateXml(totalState.createXml());
    copyXmlToBinary(*stateXml, destData);
}

void CatecophonyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore plugin state from a value tree. Currently only filenames are
    // stored in the value tree, so the corpus needs to be re-calculated each
    // load. Hope to fix this in future.
    std::unique_ptr<XmlElement> totalStateXml(
        getXmlFromBinary(data, sizeInBytes));
    
    if (totalStateXml.get() != nullptr)
    {
        auto paramStateXml = totalStateXml->getChildElement(0);
        if (paramStateXml->hasTagName(params.state.getType()))
        {
            params.replaceState(ValueTree::fromXml(*paramStateXml));
        }

        auto corpusFilesXml = totalStateXml->getChildElement(1);
        corpusFiles.copyPropertiesAndChildrenFrom(
            ValueTree::fromXml(*corpusFilesXml),
            nullptr);
        reloadCorpusFromValueTree();
    }
}
//==============================================================================

void CatecophonyAudioProcessor::setGrainAndHopSize(
    size_t grainSize,
    size_t hopSize)
{
    std::cout<<"Grain: "<<grainSize<<std::endl;
    std::cout<<"Hop: "<<hopSize<<std::endl;
    setLatencySamples(grainSize);
    this->grainSize = grainSize;
    this->hopSize = hopSize;
    initialiseBuffers();
}

void CatecophonyAudioProcessor::setWindow(Window::WindowType window)
{
    this->window = window;
}

ProcessorState CatecophonyAudioProcessor::getState()
{
    return state;
}

BufferState CatecophonyAudioProcessor::getBufferState()
{
    return bufState;
}

void CatecophonyAudioProcessor::setState(ProcessorState state)
{
    this->state = state;
}

void CatecophonyAudioProcessor::setCorpus(std::unique_ptr<GrainCorpus> corpus)
{
    this->corpus = std::move(corpus);
}

GrainCorpus* CatecophonyAudioProcessor::getCorpus()
{
    return corpus.get();
}

void CatecophonyAudioProcessor::setFeatureExtractorChain(
    std::unique_ptr<FeatureExtractorChain> featureExtractorChain)
{
    this->featureExtractorChain = std::move(featureExtractorChain);
}

FeatureExtractorChain* CatecophonyAudioProcessor::getFeatureExtractorChain()
{
    return featureExtractorChain.get();
}

void CatecophonyAudioProcessor::initialiseCorpusFromFilenames(
    const StringArray& files,
    std::function<void()> finishedCallback)
{
    addFilenamesToValueTree(files);
    auto workingGrainSize = getSelectedGrainSize();
    auto workingHopSize = getSelectedHopSize();
    auto workingFeatures = getSelectedFeatures();
    auto workingWindow = getSelectedWindow();

    worker.reset(
        new AnalysisWorker(
            *this,
            files,
            workingGrainSize,
            workingHopSize,
            workingFeatures,
            workingWindow,
            finishedCallback));
    worker->startThread(2);
}

void CatecophonyAudioProcessor::analyseCorpus(
    std::function<void()> finishedCallback)
{
    if (state != ProcessorState::Ready)
    {
        return;
    }
    reloadCorpusFromValueTree();
}

float CatecophonyAudioProcessor::getWorkerProgress()
{
    return worker->getProgress();
}

void CatecophonyAudioProcessor::initialiseBuffers()
{
    // Dynamically allocate and zero buffers to prevent weird uninitialised
    // memory issues
    grainBuffer = new float*[2];
    for (int c = 0; c < 2; c++)
    {
        grainBuffer[c] = new float[grainSize];
        for (int n = 0; n < grainSize; n++)
        {
            grainBuffer[c][n] = 0.0f;
        }
    }
    grainBufferWritePointer = 0;

    outputBuffer = new float*[2];
    for (int c = 0; c < 2; c++)
    {
        outputBuffer[c] = new float[grainSize + hopSize];
        for (int n = 0; n < grainSize + hopSize; n++)
        {
            outputBuffer[c][n] = 0.0f;
        }
    }
    outputBufferReadPointer = grainSize;
    outputBufferWritePointer = 0;

    nextGrain = new float*[2];
    for (int c = 0; c < 2; c++)
    {
        nextGrain[c] = new float[grainSize];
        for (int n = 0; n < grainSize; n++)
        {
            nextGrain[c][n] = 0.0f;
        }
    }
}

Window::WindowType CatecophonyAudioProcessor::getSelectedWindow()
{
    AudioParameterChoice* windowParam;
    windowParam = dynamic_cast<AudioParameterChoice*>(
        params.getParameter("window_type"));
    auto windowName = windowParam->getCurrentChoiceName();
    if (windowName == "Rectangular")
        return Window::Rectangular;
    else if (windowName == "Triangular")
        return Window::Triangular;
    else if (windowName == "Hann")
        return Window::Hann;
    else if (windowName == "Hamming")
        return Window::Hamming;
    else
        return Window::Hann;
}

Array<Feature> CatecophonyAudioProcessor::getSelectedFeatures()
{
    Array<AudioParameterChoice*> featureParams;
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_1")));
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_2")));
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_3")));
    StringArray addedFeatureNames;
    
    Array<Feature> features;

    for (auto featureParam : featureParams)
    {
        auto featureName  = featureParam->getCurrentChoiceName();

        bool skip = false;
        for (auto& addedFeature : addedFeatureNames)
            if (featureName == addedFeature)
                skip = true;
        if (skip) continue;

        if (featureName != "None")
        {
            auto feature = getExtractorByString(featureName);
            features.add(feature);
            addedFeatureNames.add(featureName);
        }
    }

    return features;
}

int CatecophonyAudioProcessor::getSelectedGrainSize()
{
    auto grainSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("grainSize"));
    auto grainSize = (int)powf(2.0, *grainSizeParam + 1);

    return grainSize;
}

int CatecophonyAudioProcessor::getSelectedHopSize()
{
    auto hopSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("hopSize"));
    auto hopSize = (int)powf(2.0, *hopSizeParam + 1);

    return hopSize;
}

void CatecophonyAudioProcessor::addFilenamesToValueTree(
    const StringArray& files)
{
    ValueTree vt("CorpusFiles");

    for (auto& file : files)
    {
        ValueTree fileTree("File");
        fileTree.setProperty("path", file, nullptr);
        vt.appendChild(fileTree, nullptr);
    }

    corpusFiles.copyPropertiesAndChildrenFrom(vt, nullptr);
}

void CatecophonyAudioProcessor::reloadCorpusFromValueTree()
{
    StringArray filePaths;
    for (auto child : corpusFiles)
    {
        auto filePath = child.getProperty("path").toString();
        filePaths.add(filePath);
    }
    initialiseCorpusFromFilenames(filePaths, [](){});
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CatecophonyAudioProcessor();
}
