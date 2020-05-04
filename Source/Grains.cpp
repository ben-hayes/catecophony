/*
  ==============================================================================

    Grains.cpp
    Created: 28 Apr 2020 11:46:13am
    Author:  Ben Hayes

  ==============================================================================
*/

#include "Grains.h"

Window::Window(Window::WindowType windowType, int size)
    : buffer(1, size) // window buffer only needs one channel
{
    switch (windowType)
    {
        case Rectangular:
            makeRectangular();
        break;

        case Triangular:
            makeTriangular();
        break;

        case Hann:
            makeHann();
        break;

        case Hamming:
            makeHamming();
        break;

        default:
            makeRectangular();
    }
}

void Window::makeRectangular()
{
    int N = buffer.getNumSamples();

    for (int n = 0; n < N; n++)
    {
        buffer.setSample(0, n, 1.0f);
    }
}

void Window::makeTriangular()
{
    int N = buffer.getNumSamples();

    for (int n = 0; n < N; n++)
    {
        auto w_n = 1.0f - fabsf(n / (0.5f * N) - 1.0f);
        buffer.setSample(0, n, w_n);
    }
}

void Window::makeHann()
{
    int N = buffer.getNumSamples();

    for (int n = 0; n < N; n++)
    {
        auto w_n = powf(sinf(M_PI * n / N), 2.0f);
        buffer.setSample(0, n, w_n);
    }
}

void Window::makeHamming()
{
    int N = buffer.getNumSamples();

    for (int n = 0; n < N; n++)
    {
        auto w_n = 0.54347826f - 0.45652174f * cosf(2.0f * M_PI * n / N);
        buffer.setSample(0, n, w_n);
    }
}

float Window::operator[](size_t i)
{
    return buffer.getSample(0, i);
}

const float Window::operator[](size_t i) const
{
    return buffer.getSample(0, i);
}

Grain::Grain()
    : buffer(std::make_unique<AudioBuffer<float>>()),
      monoBuffer(std::make_unique<AudioBuffer<float>>())
{
}

Grain::Grain(
    std::unique_ptr<AudioBuffer<float>> audioBuffer,
    Window::WindowType windowType)
    : buffer(std::move(audioBuffer)),
      monoBuffer(std::make_unique<AudioBuffer<float>>())
{
    if (buffer->getNumChannels() != 2)
    {
        float** newRawBuffer = new float*[2];
        newRawBuffer[0] = buffer->getWritePointer(0);
        newRawBuffer[1] = buffer->getWritePointer(0);
        auto newBuffer = std::make_unique<AudioBuffer<float>>(
            newRawBuffer,
            2,
            buffer->getNumSamples());
        buffer = std::move(newBuffer);
    }
    applyWindow(windowType);
    setMagnitude();
    makeMonoBuffer();
}

void Grain::init(
    float** data,
    int numChannels,
    int lengthInSamples,
    Window::WindowType windowType)
{
    buffer->setDataToReferTo(data, numChannels, lengthInSamples);
    setMagnitude();
    applyWindow(windowType);
    makeMonoBuffer();
}

float Grain::operator[](size_t i)
{
    return buffer->getSample(0, i);
}

const float Grain::operator[](size_t i) const
{
    return buffer->getSample(0, i);
}

const float* Grain::getRawMonoBuffer()
{
    return monoBuffer->getReadPointer(0);
}

float** Grain::getRawBuffer()
{
    return buffer->getArrayOfWritePointers();
}

size_t Grain::getBufferLength()
{
    return buffer->getNumSamples();
}

void Grain::applyWindow(Window::WindowType windowType)
{
    Window w(windowType, buffer->getNumSamples());

    for (int n = 0; n < buffer->getNumSamples(); n++)
    {
        buffer->applyGain(n, 1, w[n]);
    }
}

bool Grain::isSilent(float tolerance)
{
    return mag < tolerance;
}

float Grain::getMagnitude()
{
    return mag;
}

void Grain::makeMonoBuffer()
{
    monoBuffer->setSize(1, buffer->getNumSamples());
    monoBuffer->clear();

    for (int channel = 0; channel < buffer->getNumChannels(); channel++)
    {
        monoBuffer->addFrom(0, 0, *buffer, channel, 0, buffer->getNumSamples());
    }
    monoBuffer->applyGain(1.0f / buffer->getNumChannels());
}

void Grain::setMagnitude()
{
    mag = buffer->getMagnitude(0, buffer->getNumSamples());
}