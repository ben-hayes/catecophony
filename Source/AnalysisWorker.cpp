/*
  ==============================================================================

    AnalysisWorker.cpp
    Created: 1 May 2020 6:04:15pm
    Author:  Ben Hayes

  ==============================================================================
*/

#include "AnalysisWorker.h"

AnalysisWorker::AnalysisWorker(
    CatecophonyAudioProcessor& processor,
    const StringArray files,
    int grainSize,
    int hopSize,
    Array<Feature> features)
    : Thread("CatecophonyCorpusWorker"),
      processor(processor),
      files(files),
      grainSize(grainSize),
      hopSize(hopSize),
      features(features),
      loadFiles(true)
{
    formatManager.registerBasicFormats();
}

AnalysisWorker::AnalysisWorker(
    CatecophonyAudioProcessor& processor,
    int grainSize,
    int hopSize,
    Array<Feature> features)
    : Thread("CatecophonyAnalysisOnlyWorker"),
      processor(processor),
      grainSize(grainSize),
      hopSize(hopSize),
      features(features),
      loadFiles(false)
{
    formatManager.registerBasicFormats();
}

AnalysisWorker::~AnalysisWorker() {}

void AnalysisWorker::run()
{
    if (loadFiles) loadCorpusFiles();
    analyseCorpus();
}

void AnalysisWorker::loadCorpusFiles()
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
    
    auto corpus = std::make_unique<GrainCorpus>(
        audioReaders,
        Window::Hann,
        grainSize,
        hopSize);
    processor.setGrainAndHopSize(grainSize, hopSize);
    processor.setCorpus(std::move(corpus));
}

void AnalysisWorker::analyseCorpus()
{
    if (processor.getState() == ProcessorState::NoCorpus) return;

    processor.setState(ProcessorState::Analysing);
    // Wait for buffers to be free
    while (processor.getBufferState() != BufferState::NotInUse) {}

    auto* corpus = processor.getCorpus();
    auto featureExtractorChain =
        std::make_unique<FeatureExtractorChain>(corpus->getGrainLength());
    
    for (auto feature : features)
    {
        featureExtractorChain->addFeatureExtractor(feature);
    }
    corpus->analyse(featureExtractorChain.get());
    processor.setFeatureExtractorChain(std::move(featureExtractorChain));
    processor.setState(ProcessorState::Ready);
}