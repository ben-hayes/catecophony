/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    AnalysisWorker.cpp
    Description: Worker thread for analysing corpus - implementation
  ==============================================================================
*/

#include "AnalysisWorker.h"

AnalysisWorker::AnalysisWorker(
    CatecophonyAudioProcessor& processor,
    const StringArray files,
    int grainSize,
    int hopSize,
    Array<Feature> features,
    Window::WindowType window,
    std::function<void()> finishedCallback)
    : Thread("CatecophonyCorpusWorker"),
      processor(processor),
      files(files),
      grainSize(grainSize),
      hopSize(hopSize),
      features(features),
      window(window),
      loadFiles(true),
      finishedCallback(finishedCallback)
{
    formatManager.registerBasicFormats();
}

AnalysisWorker::AnalysisWorker(
    CatecophonyAudioProcessor& processor,
    int grainSize,
    int hopSize,
    Array<Feature> features,
    Window::WindowType window,
    std::function<void()> finishedCallback)
    : Thread("CatecophonyAnalysisOnlyWorker"),
      processor(processor),
      grainSize(grainSize),
      hopSize(hopSize),
      features(features),
      window(window),
      loadFiles(false),
      finishedCallback(finishedCallback)
{
    formatManager.registerBasicFormats();
}

AnalysisWorker::~AnalysisWorker()
{
    stopThread(2000);
}

void AnalysisWorker::run()
{
    if (loadFiles) loadCorpusFiles();
    analyseCorpus();
    // report our progress as finished
    progress = -1.0f;

    // We stored a callback in the constructor, now let's call it
    finishedCallback();
}

void AnalysisWorker::loadCorpusFiles()
{
    // Change the processor state
    processor.setState(ProcessorState::LoadingFiles);
    Array<std::unique_ptr<AudioFormatReader>> audioReaders; 

    auto i = 0;
    for (auto& file : files)
    {
        // For each file, try to create an appropriate format reader
        std::unique_ptr<AudioFormatReader> reader (
            formatManager.createReaderFor(file));
        
        if (reader)
        {
            audioReaders.add(std::move(reader));
        } else
        {
            std::cout<<"No reader!"<<std::endl;
        }
        i++;
        progress = 0.25f * (float)i / files.size();
    }

    // Wait for buffers to be free
    while (processor.getBufferState() != BufferState::NotInUse) {}
    
    // Now build a corpus from our files -- this means granulating each one
    auto corpus = std::make_unique<GrainCorpus>(
        audioReaders,
        window,
        grainSize,
        hopSize);
    progress = 0.5f;
    // Pass the corpus params to our processor
    processor.setGrainAndHopSize(grainSize, hopSize);
    processor.setWindow(window);
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
    corpus->analyse(
        featureExtractorChain.get(),
        [this](float val) {
            // This callback lets us report progress as we go. Nice for the
            // loading bar
            if (loadFiles) progress = 0.5 + 0.5 * val;
            else progress = val;
        });
    // Pass the feature extractor chain back to the processor for realtime use
    processor.setFeatureExtractorChain(std::move(featureExtractorChain));
    processor.setState(ProcessorState::Ready);
}