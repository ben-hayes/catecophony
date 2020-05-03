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
            initialiseCorpusFromFilenames(files);
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

    switch (processor.getState())
    {
        case ProcessorState::Ready: {
            gui->startGrainAnimation(
                processor.getCorpus()->get3DGrainCoords(),
                [this](){
                    return processor.getCorpus()->getMatchHistory();
                });
            break;
        }
        case ProcessorState::LoadingFiles: {
            gui->startLoading([this](){
                return processor.getWorkerProgress();
            });
            break;
        }

        case ProcessorState::Analysing: {
            gui->startLoading([this](){
                return processor.getWorkerProgress();
            });
            break;
        }

        case ProcessorState::NoCorpus: {
            break;
        }
    } 
}

CatecophonyAudioProcessorEditor::~CatecophonyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void CatecophonyAudioProcessorEditor::paint (Graphics& g)
{
    if (processor.getState() == ProcessorState::Ready
        && !gui->animationIsRunning())
    {
        gui->startGrainAnimation(
            processor.getCorpus()->get3DGrainCoords(),
            [this](){
                return processor.getCorpus()->getMatchHistory();
            });
    }
}

void CatecophonyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void CatecophonyAudioProcessorEditor::initialiseCorpusFromFilenames(
    const StringArray& files)
{
    processor.initialiseCorpusFromFilenames(
        files,
        [this](){
            gui->startGrainAnimation(
                processor.getCorpus()->get3DGrainCoords(),
                [this](){
                    return processor.getCorpus()->getMatchHistory();
                });
            gui->stopLoading();
        });
    gui->startLoading([this](){
        return processor.getWorkerProgress();
    });
    gui->stopGrainAnimation();
}

void CatecophonyAudioProcessorEditor::analyseCorpus()
{
    processor.analyseCorpus(
        [this](){
            gui->startGrainAnimation(
                processor.getCorpus()->get3DGrainCoords(),
                [this](){
                    return processor.getCorpus()->getMatchHistory();
                });
            gui->stopLoading();
        });
    gui->startLoading([this](){
        return processor.getWorkerProgress();
    });
    gui->stopGrainAnimation();
}