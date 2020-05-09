# Catecophony

Catecophony is a VST3/AU plugin that performs real-time corpus-based concatenative synthesis. Given some source audio (the corpus) which is split into thousands of grains and analysed, it attempts to recreate an incoming audio stream (the target) using the corpus grains. The result is pointillist sound shapes that are often chaotic and sometimes lovely.

More info and plugin download available here: https://ben-hayes.github.io/

Catecophony is built on the [_JUCE_](https://github.com/juce-framework/JUCE) framework and uses [_Essentia_](https://github.com/MTG/essentia) and [_FFTW3_](http://fftw.org/) for feature extraction.

![Catecophony GUI](https://ben-hayes.github.io/catecophony.png)
