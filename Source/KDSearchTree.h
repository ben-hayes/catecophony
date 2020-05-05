/*
  ==============================================================================
    Ben Hayes
    ECS730P - Digital Audio Effects
    KDSearchTree.h
    Description: The data structure used to implement the nearest neighbour
        search efficiently
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class KDSearchTree
{
public:
    // Public facing constructor
    KDSearchTree(const Array<Array<float>>& points, int startAxis = 0)
        : points(points)
    {
        Array<int> pointIndices;
        // create an ordered list of indices. This will be passed down and
        // diced up throughout the tree
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
        // recursive search for the best matching point
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

    // JUCE's array sort allows a comparator object to be passed in. Making it
    // stateful allows us to avoid copying the whole points array.
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
        // If we're at the bottom of a tree, we don't need any more subnodes
        if (pointIndices.size() == 1)
        {
            terminalNode = true;
            pointIndex = pointIndices[0];
            return;
        }

        splitAxis = axis;

        // otherwise, find the middle, and create two trees either side
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
        
        // and do so recursively
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