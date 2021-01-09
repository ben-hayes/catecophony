# Catecophony

Catecophony is a VST3/AU plugin that performs real-time corpus-based concatenative synthesis. Given some source audio (the corpus) which is split into thousands of grains and analysed, it attempts to recreate an incoming audio stream (the target) using the corpus grains. The result is pointillist sound shapes that are often chaotic and sometimes lovely.

More info: https://vimeo.com/415074832

Plugin download available here: https://github.com/ben-hayes/catecophony/releases/tag/v0.0.1-alpha

Catecophony is built on the [_JUCE_](https://github.com/juce-framework/JUCE) framework and uses [_Essentia_](https://github.com/MTG/essentia) and [_FFTW3_](http://fftw.org/) for feature extraction.
