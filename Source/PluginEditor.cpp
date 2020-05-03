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

void CatecophonyAudioProcessorEditor::initialiseCorpusFromFilenames(
    const StringArray& files)
{
    auto grainSize = getSelectedGrainSize();
    auto hopSize = getSelectedHopSize();
    auto features = getSelectedFeatures();

    worker.reset(
        new AnalysisWorker(
            processor,
            files,
            grainSize,
            hopSize,
            features,
            [this](){
                gui->startGrainAnimation(
                    processor.getCorpus()->get3DGrainCoords(),
                    [this](){
                        return processor.getCorpus()->getMatchHistory();
                    });
                gui->stopLoading();
            }));
    gui->startLoading([this](){
        return worker->getProgress();
    });
    gui->stopGrainAnimation();
    worker->startThread(2);
}

void CatecophonyAudioProcessorEditor::analyseCorpus()
{
    if (processor.getState() != ProcessorState::Ready)
    {
        return;
    }

    auto grainSize = getSelectedGrainSize();
    auto hopSize = getSelectedHopSize();
    auto features = getSelectedFeatures();

    worker.reset(
        new AnalysisWorker(
            processor,
            grainSize,
            hopSize,
            features,
            [this](){
                gui->startGrainAnimation(
                    processor.getCorpus()->get3DGrainCoords(),
                    [this](){
                        return processor.getCorpus()->getMatchHistory();
                    });
                gui->stopLoading();
            }));
    gui->startLoading([this](){
        return worker->getProgress();
    });
    gui->stopGrainAnimation();
    worker->startThread(2);
}

Array<Feature> CatecophonyAudioProcessorEditor::getSelectedFeatures()
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

int CatecophonyAudioProcessorEditor::getSelectedGrainSize()
{
    auto grainSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("grainSize"));
    auto grainSize = (int)powf(2.0, *grainSizeParam + 1);

    return grainSize;
}

int CatecophonyAudioProcessorEditor::getSelectedHopSize()
{
    auto hopSizeParam = dynamic_cast<AudioParameterInt*>(
        params.getParameter("hopSize"));
    auto hopSize = (int)powf(2.0, *hopSizeParam + 1);

    return hopSize;
}