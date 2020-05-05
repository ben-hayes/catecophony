/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    Grains.h
    Description: Header file for helpful grain classes
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
    Grain();
    Grain(
        std::unique_ptr<AudioBuffer<float>> audioBuffer,
        Window::WindowType windowType = Window::Rectangular);
    
    void init(
        float** data,
        int numChannels,
        int lengthInSamples,
        Window::WindowType windowType = Window::Rectangular);

    float operator[](size_t i);
    const float operator[](size_t i) const;

    const float* getRawMonoBuffer();
    float** getRawBuffer();
    size_t getBufferLength();

    bool isSilent(float tolerance = 0.00001);
    float getMagnitude();

private:
    std::unique_ptr<AudioBuffer<float>> buffer;
    std::unique_ptr<AudioBuffer<float>> monoBuffer;

    void applyWindow(Window::WindowType windowType);
    void makeMonoBuffer();

    void setMagnitude();

    float mag;
};