/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <functional>

#include "GrainCorpus.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CatecophonyAudioProcessorEditor::CatecophonyAudioProcessorEditor (
    CatecophonyAudioProcessor& p,
    AudioProcessorValueTreeState& v)
    : AudioProcessorEditor (&p), processor (p), params(v)
{
    setSize (800, 600);
    setLookAndFeel(&skin);

    formatManager.registerBasicFormats();
    std::unique_ptr<WildcardFileFilter> fileFilter(
        new WildcardFileFilter(
            formatManager.getWildcardForAllFormats(),
            {},
            {})); 

    gui.reset(new MainGui(v, std::move(fileFilter)));
    gui->setBounds(0, 0, 800, 600);
    gui->setFileDropCallback(
        [this](const StringArray& files) {
            loadCorpusFiles(files);
        });
    gui->setAnalyseCallback(
        [this]() {
            analyseCorpus();
        });
    addAndMakeVisible(gui.get());

    getLookAndFeel().setColour(
        ResizableWindow::backgroundColourId,
        Colour(
            (0xff << 24)
            | (0x21 << 16)
            | (0x24 << 8)
            | (0x3d)
        ));
}

CatecophonyAudioProcessorEditor::~CatecophonyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void CatecophonyAudioProcessorEditor::paint (Graphics& g)
{
}

void CatecophonyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void CatecophonyAudioProcessorEditor::loadCorpusFiles(
    const StringArray& files)
{
    processor.setState(ProcessorState::LoadingFiles);
    Array<std::unique_ptr<AudioFormatReader>> audioReaders; 

    for (auto file : files)
    {
        std::unique_ptr<AudioFormatReader> reader (
            formatManager.createReaderFor(file));
        
        if (reader)
        {
            audioReaders.add(std::move(reader));
        }
    }

    // Wait for buffers to be free
    while (processor.getBufferState() != BufferState::NotInUse) {}

    auto grainSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("grainSize"));
    auto hopSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("hopSize"));
    auto grainSize = (int)powf(2.0, *grainSizeParam);
    auto hopSize = (int)powf(2.0, *hopSizeParam);
    
    // NOTE TO SELF -- left off here... need to pass newg
    // grain size and hop size to processor
    auto corpus = std::make_unique<GrainCorpus>(
        audioReaders,
        Window::Hann,
        grainSize,
        hopSize);
    processor.setGrainAndHopSize(grainSize, hopSize);
    processor.setCorpus(std::move(corpus));

    analyseCorpus();
}

void CatecophonyAudioProcessorEditor::analyseCorpus()
{
    if (processor.getState() == ProcessorState::NoCorpus) return;

    processor.setState(ProcessorState::Analysing);
    // Wait for buffers to be free
    while (processor.getBufferState() != BufferState::NotInUse) {}

    auto* corpus = processor.getCorpus();
    auto featureExtractorChain =
        std::make_unique<FeatureExtractorChain>(corpus->getGrainLength());
    
    Array<AudioParameterChoice*> featureParams;
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_1")));
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_2")));
    featureParams.add(dynamic_cast<AudioParameterChoice*>(
        params.getParameter("feature_3")));

    for (auto featureParam : featureParams)
    {
        if (featureParam->getCurrentChoiceName() != "None")
        {
            std::cout<<featureParam->getCurrentChoiceName()<<std::endl;
            auto feature = getExtractorByString(
                featureParam->getCurrentChoiceName());
            featureExtractorChain->addFeatureExtractor(feature);
        }
    }
    corpus->analyse(featureExtractorChain.get());
    processor.setFeatureExtractorChain(std::move(featureExtractorChain));
    processor.setState(ProcessorState::Ready);
}