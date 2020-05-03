/*
  ==============================================================================

    GrainView.cpp
    Created: 2 May 2020 2:22:37pm
    Author:  Ben Hayes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GrainView.h"

//==============================================================================
GrainView::GrainView(int maxGrainsToShow)
    : maxGrainsToShow(maxGrainsToShow),
      state(AnimationState::NotRunning)
{
    angleStep = 2.0f * M_PI * 0.1 * 0.0417;
    currentAngle = 0.0f;
    grainCoords.reset(new Array<Array<float>>());

    startTimerHz(15);
}

GrainView::~GrainView()
{
}

void GrainView::paint (Graphics& g)
{
    if (!hasGrainCoords || state == AnimationState::NotRunning) return;
    auto w = getWidth();
    auto h = getHeight();
    for (auto idx : grainIndices)
    {
        auto grain = (*grainCoords)[idx];
        auto coords = rotateAndProjectPoint(
            grain[0],
            grain[1],
            grain[2],
            currentAngle);

        if (coords.x < -1.5f
                || coords.x > 1.5f
                || coords.y < -1.5f
                || coords.y > 1.5f
                || isnan(coords.x)
                || isnan(coords.y)
                || isnan(coords.z)
                || isinf(coords.x)
                || isinf(coords.y)
                || isinf(coords.z))
            continue;

        auto diameter = 5 * (0.5f * coords.z + 0.5f) + 5;
        if (diameter < 1) diameter = 1;
        g.setColour(Colour(
            (0xbb << 24)
            | (0x88 << 16)
            | (0xe1 << 8)
            | (0xff)));
        g.fillEllipse(
            0.5 * w * coords.x + getLocalBounds().getCentreX(),
            0.5 * h * coords.y + getLocalBounds().getCentreY(),
            diameter,
            diameter);
    }

    auto matchedGrains = matchCallback();
    for (auto idx : matchedGrains)
    {
        auto grain = (*grainCoords)[idx];
        auto coords = rotateAndProjectPoint(
            grain[0],
            grain[1],
            grain[2],
            currentAngle);

        if (coords.x < -1.5f
                || coords.x > 1.5f
                || coords.y < -1.5f
                || coords.y > 1.5f
                || isnan(coords.x)
                || isnan(coords.y)
                || isnan(coords.z)
                || isinf(coords.x)
                || isinf(coords.y)
                || isinf(coords.z))
            continue;

        auto size = 8 * (0.5f * coords.z + 0.5f) + 8;
        if (size < 1) size = 1;
        g.setColour(Colour(
            (0xff << 24)
            | (0xff << 16)
            | (0x7c << 8)
            | (0x7c)));
        g.fillRect(
            0.5 * w * coords.x + getLocalBounds().getCentreX(),
            0.5 * h * coords.y + getLocalBounds().getCentreY(),
            size,
            size);
    }

    currentAngle += angleStep;
    while (currentAngle >= 2.0f * M_PI) currentAngle -= 2.0f * M_PI;
}

void GrainView::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GrainView::setGrainCoords(
    std::unique_ptr<Array<Array<float>>> newGrainCoords)
{
    grainCoords = std::move(newGrainCoords);
    hasGrainCoords = true;
    computeMeanAndStd();
    computeGrainIndices();
    state = AnimationState::Running;
}

void GrainView::setMatchCallback(std::function<Array<int>()> matchCallback)
{
    this->matchCallback = matchCallback;
}

bool GrainView::animationIsRunning()
{
    return state == AnimationState::Running;
}

void GrainView::computeMeanAndStd()
{
    for (int i = 0; i < 3; i++)
    {
        mean[i] = 0.0f;
        stdev[i] = 0.0f;
    }

    for (auto& grain : *grainCoords)
    {
        for (int i = 0; i < 3; i ++)
        {
            mean[i] += grain[i];
        }
    }

    for (int i = 0; i < 3; i ++)
    {
        mean[i] /= grainCoords->size();
    }

    for (auto& grain : *grainCoords)
    {
        for (int i = 0; i < 3; i ++)
        {
            stdev[i] += powf(mean[i] - grain[i], 2.0f);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        stdev[i] /= grainCoords->size() - 1;
        stdev[i] = sqrtf(stdev[i]);

        if (stdev[i] == 0.0) stdev[i] = 1.0;
    }
}

void GrainView::computeGrainIndices()
{
    grainIndices.clear();
    for (
        int i = 0;
        i < (maxGrainsToShow < grainCoords->size()
            ? maxGrainsToShow : grainCoords->size());
        i++)
    {
        auto idx = rand() % grainCoords->size();
        grainIndices.add(idx);
    }
}

Point3D GrainView::rotateAndProjectPoint(float x, float y, float z, float theta)
{
    Point3D out;
    auto X = (x - mean[0]) / stdev[0];
    auto Y = (y - mean[1]) / stdev[1];
    auto Z = (z - mean[2]) / stdev[2];

    auto cosTheta = cosf(theta);
    auto sinTheta = sinf(theta);

    auto rotX = cosTheta * X + sinTheta * Z;
    auto rotY = Y;
    auto rotZ = - sinTheta * X + cosTheta * Z;

    auto zBase = 3.0f - rotZ;
    if (zBase < 0.0f) zBase = 0.0f;
    auto phiX = atan2f(rotX, zBase);
    auto phiY = atan2f(rotY, zBase);
    auto projX = 1.5 * tanf(phiX);
    auto projY = 1.5 * tanf(phiY);

    out.x = projX;
    out.y = projY;
    out.z = rotZ;

    return out;
}