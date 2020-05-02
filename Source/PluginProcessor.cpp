/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

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
        outputBufferReadPointer(MAX_BUFFER_SIZE - hopSize),
        params(*this, nullptr, "Catecophony", {
                std::make_unique<AudioParameterFloat>(
                    "drywet", "Dry/Wet",
                    0.0f, 1.0f, 1.0f),
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
                std::make_unique<AudioParameterChoice>(
                    "feature_1", "Feature #1",
                    StringArray({
                        "Dissonance",
                        "F0",
                        "Inharmonicity",
                        "MFCC",
                        "Odd:even Harmonic Ratio",
                        "Pitch Salience",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Complexity",
                        "Spectral Contrast",
                        "Spectral Flatness",
                        "Spectral Peaks",
                        "Spectral Roll-off",
                        "Strong Peak Ratio",
                        "Zero Crossing Rate"
                    }),
                    7),
                std::make_unique<AudioParameterChoice>(
                    "feature_2", "Feature #2",
                    StringArray({
                        "None",
                        "Dissonance",
                        "F0",
                        "Inharmonicity",
                        "MFCC",
                        "Odd:even Harmonic Ratio",
                        "Pitch Salience",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Complexity",
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
                        "Dissonance",
                        "F0",
                        "Inharmonicity",
                        "MFCC",
                        "Odd:even Harmonic Ratio",
                        "Pitch Salience",
                        "RMS",
                        "Spectral Centroid",
                        "Spectral Complexity",
                        "Spectral Contrast",
                        "Spectral Flatness",
                        "Spectral Peaks",
                        "Spectral Roll-off",
                        "Strong Peak Ratio",
                        "Zero Crossing Rate"
                    }),
                    11)    
        })
{

    dryWet = params.getRawParameterValue("drywet");
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

void CatecophonyAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (state != ProcessorState::Ready) return;

    bufState = BufferState::InUse;
    for (int n = 0; n < buffer.getNumSamples(); n++)
    {
        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            grainBuffer[channel][grainBufferWritePointer] =
                buffer.getSample(channel, n);
        }

        grainBufferWritePointer += 1;
        if (grainBufferWritePointer >= MAX_BUFFER_SIZE)
        {
            grainBufferWritePointer = 0;
        }

        hopCounter += 1;
        if (hopCounter >= hopSize)
        {
            hopCounter = 0;
            for (int i = 0; i < grainSize; i++)
            {
                for (int c = 0; c < 2; c++)
                {
                    nextGrain[c][i] = grainBuffer[c][
                        (grainBufferWritePointer 
                        + MAX_BUFFER_SIZE 
                        - grainSize 
                        + i) % MAX_BUFFER_SIZE];
                }
            }

            workingGrain.init(nextGrain, 2, grainSize, Window::Hann);

            if (!workingGrain.isSilent())
            {
                auto matchGain = dynamic_cast<AudioParameterBool*>(
                    params.getParameter("matchGain"))->get();
                auto gainScale = matchGain ? workingGrain.getMagnitude() : 1.0f;
                auto features = featureExtractorChain->process(&workingGrain);
                auto* nearestGrain = corpus->findNearestStep(features);
                auto** rawGrainBuffer = nearestGrain->getRawBuffer();

                for (int i = 0; i < grainSize; i++)
                {
                    for (int c = 0; c < 2; c++)
                    {
                        outputBuffer[c][
                            (outputBufferWritePointer + i) % MAX_BUFFER_SIZE] += 
                                rawGrainBuffer[c][i] * gainScale;
                    }
                }
            } else
            {
                corpus->resetStepChain();
            }

            outputBufferWritePointer += hopSize;
            if (outputBufferWritePointer >= MAX_BUFFER_SIZE)
            {
                outputBufferWritePointer =
                    outputBufferWritePointer % MAX_BUFFER_SIZE;
            }
        }

        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            auto inSample = buffer.getSample(channel, n);
            auto out = *dryWet * outputBuffer[channel][outputBufferReadPointer]
                + (1.0f - *dryWet) * inSample;
            buffer.setSample(
                channel,
                n,
                out);
            outputBuffer[channel][outputBufferReadPointer] = 0.0f;
        }

        outputBufferReadPointer += 1;
        if (outputBufferReadPointer >= MAX_BUFFER_SIZE)
        {
            outputBufferReadPointer = 0;
        }
    }
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CatecophonyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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

void CatecophonyAudioProcessor::initialiseBuffers()
{
    nextGrain = new float*[2];
    for (int c = 0; c < 2; c++)
    {
        nextGrain[c] = new float[grainSize];
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CatecophonyAudioProcessor();
}
