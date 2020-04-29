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
CatecophonyAudioProcessorEditor::CatecophonyAudioProcessorEditor (CatecophonyAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (800, 600);
    setLookAndFeel(&skin);

    formatManager.registerBasicFormats();
    std::unique_ptr<WildcardFileFilter> fileFilter(
        new WildcardFileFilter(
            formatManager.getWildcardForAllFormats(),
            {},
            {})); 

    gui.reset(new MainGui(std::move(fileFilter)));
    gui->setBounds(0, 0, 800, 600);
    gui->setFileDropCallback(
        [this](const StringArray& files) {
            loadCorpusFiles(files);
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
    auto corpus = std::make_unique<GrainCorpus>(audioReaders);
    processor.setCorpus(std::move(corpus));

    analyseCorpus();
}

void CatecophonyAudioProcessorEditor::analyseCorpus()
{
    auto corpus = processor.getCorpus();
    auto featureExtractorChain =
        std::make_unique<FeatureExtractorChain>(corpus->getGrainLength());
    
    featureExtractorChain->addFeatureExtractor(SpectralCentroid);

    corpus->analyse(featureExtractorChain.get());
    processor.setFeatureExtractorChain(std::move(featureExtractorChain));
}