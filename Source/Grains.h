/*
  ==============================================================================

    Grains.h
    Created: 28 Apr 2020 11:46:13am
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Window
{
public:
    enum WindowType {
        Rectangular = 0,
        Triangular,
        Hann,
        Hamming
    };

    Window(WindowType, int size);

    void makeRectangular();
    void makeTriangular();
    void makeHann();
    void makeHamming();

    float operator[](size_t i);
    const float operator[](size_t i) const;

private:
    AudioBuffer<float> buffer;
};

class Grain
{
public:
    Grain(
        std::unique_ptr<AudioBuffer<float>> audioBuffer,
        Window::WindowType windowType = Window::Rectangular);

    float operator[](size_t i);
    const float operator[](size_t i) const;

    const float* getRawMonoBuffer();
    size_t getBufferLength();

private:
    std::unique_ptr<AudioBuffer<float>> buffer;
    std::unique_ptr<AudioBuffer<float>> monoBuffer;

    void applyWindow(Window::WindowType windowType);
    void makeMonoBuffer();
};