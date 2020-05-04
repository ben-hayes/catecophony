/*
  ==============================================================================

    KDSearchTree.h
    Created: 3 May 2020 9:32:05pm
    Author:  Ben Hayes

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class KDSearchTree
{
public:
    KDSearchTree(const Array<Array<float>>& points, int startAxis = 0)
        : points(points)
    {
        Array<int> pointIndices;
        for (int i = 0; i < points.size(); i++) pointIndices.add(i);
        createTreeNode(points, pointIndices, startAxis);
    }

    KDSearchTree(
        const Array<Array<float>>& points,
        Array<int> pointIndices,
        int axis = 0)
        : points(points)
    {
        createTreeNode(points, pointIndices, axis);
    }

    int getNearestPoint(Array<float>& pointToMatch)
    {
        if (terminalNode) return pointIndex;

        if (pointToMatch[splitAxis] <= median)
        {
            return left->getNearestPoint(pointToMatch);
        } else
        {
            return right->getNearestPoint(pointToMatch);
        }
    }

private:
    std::unique_ptr<KDSearchTree> left;
    std::unique_ptr<KDSearchTree> right;

    const Array<Array<float>>& points;

    float median;
    int splitAxis;

    bool terminalNode = false;
    int pointIndex = -1;

    struct Comparator {
        Comparator(const Array<Array<float>>& points, int axis)
            : axis(axis),
              points(points) {}

        int axis;
        const Array<Array<float>>& points;

        int compareElements(int first, int second)
        {
            if (points[first][axis] < points[second][axis]) return -1;
            else if (points[first][axis] == points[second][axis]) return 0;
            else return 1;
        }
    };

    void createTreeNode(
        const Array<Array<float>>& points,
        Array<int> pointIndices,
        int axis)
    {
        if (pointIndices.size() == 1)
        {
            terminalNode = true;
            pointIndex = pointIndices[0];
            return;
        }

        splitAxis = axis;

        auto axisMedian = getAxisMedian(points, pointIndices, axis);
        median = axisMedian;

        auto belowPointIndices = getPointsBelowMedian(
            points,
            pointIndices,
            axis,
            axisMedian);
        auto abovePointIndices = getPointsAboveMedian(
            points,
            pointIndices,
            axis,
            axisMedian);
        
        left = std::make_unique<KDSearchTree>(
            points,
            belowPointIndices,
            (axis + 1) % points[0].size());
        right = std::make_unique<KDSearchTree>(
            points,
            abovePointIndices,
            (axis + 1) % points[0].size());
    }

    float getAxisMedian(
        const Array<Array<float>>& points,
        Array<int> pointIndices,
        int axis)
    {
        Comparator comp(points, axis);
        pointIndices.sort(comp);

        auto medianIndex = pointIndices.size() * 0.5f;
        float axisMedian;
        if (floorf(medianIndex) == medianIndex)
        {
            axisMedian = points[(int)medianIndex][axis];
        } else
        {

            axisMedian = 0.5f * (points[(int)medianIndex][axis]
                                + points[((int)medianIndex + 1)
                                    % pointIndices.size()][axis]);
        }

        return axisMedian;
    }

    Array<int> getPointsBelowMedian(
        const Array<Array<float>>& points,
        Array<int> pointIndices,
        int axis,
        float median)
    {
        Array<int> belowPointIndices;
        for (int i = 0; i < pointIndices.size() / 2.0f; i++)
        {
            belowPointIndices.add(pointIndices[i]);
        }
        return belowPointIndices;
    }

    Array<int> getPointsAboveMedian(
        const Array<Array<float>>& points,
        Array<int> pointIndices,
        int axis,
        float median)
    {
        Array<int> abovePointIndices;
        for (
            int i = ceilf(pointIndices.size() / 2.0f);
            i < pointIndices.size();
            i++)
        {
            abovePointIndices.add(pointIndices[i]);
        }
        return abovePointIndices;
    }
};