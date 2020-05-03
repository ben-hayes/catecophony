/*
  ==============================================================================

    GrainView.h
    Created: 2 May 2020 2:22:37pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
struct Point3D {
    float x;
    float y;
    float z;
};

enum class AnimationState {
    NotRunning = 0,
    Running
};

class GrainView    : public Component,
                     public Timer
{
public:
    GrainView(int maxGrainsToShow = 1750);
    ~GrainView();

    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override { repaint(); }
    void setGrainCoords(std::unique_ptr<Array<Array<float>>> newGrainCoords);
    void stopAnimation() {
         hasGrainCoords = false;
         state = AnimationState::NotRunning; }
    void setMatchCallback(std::function<Array<int>()> matchCallback);

    bool animationIsRunning();

private:
    std::function<Array<int>()> matchCallback;
    AnimationState state;

    bool hasGrainCoords = false;
    std::unique_ptr<Array<Array<float>>> grainCoords;
    float mean[3];
    float stdev[3];

    int maxGrainsToShow;
    Array<int> grainIndices;

    float currentAngle;
    float angleStep;

    void computeMeanAndStd();
    void computeGrainIndices();
    Point3D rotateAndProjectPoint(float x, float y, float z, float theta);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GrainView)
};
