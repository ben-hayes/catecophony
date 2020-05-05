/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    PluginEditor.cpp
    Description: Implementation for plugin editor.
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

    // Format manager takes care of loading audio files.
    formatManager.registerBasicFormats();
    std::unique_ptr<WildcardFileFilter> fileFilter(
        new WildcardFileFilter(
            formatManager.getWildcardForAllFormats(),
            {},
            {})); 

    // Main GUI is implemented in a GUI subcomponent, auto generated by the
    // Projucer's GUI editor. I have, however, implemented some custom
    // methods to allow callbacks to be passed to included custom components
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

    // Make sure we're showing the correct thing -- helpful if we've reloaded
    // or restored state.
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
}

// These two methods pass requests to our analysis worker
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