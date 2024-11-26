#include "pch.h"
#include "../pch.h"

#include "BezierManager.h"

#include <numeric>
#include <vector>
#include <unordered_map>

namespace g3d
{
    static std::unordered_map<CubicBezier, std::pair<std::vector<double>, double>, CubicBezierHash> arcLengths;

    void BezierManager::clearCache() { arcLengths.clear(); }

    void BezierManager::computeBezierCoordinateSegments(const CubicBezier& segment, const int segmentCount)
    {
        if (arcLengths.find(segment) == arcLengths.end())
        {
            arcLengths[segment] = std::pair<std::vector<double>, double>();
            computeArcLengths(
                segment.x0, segment.y0,
                segment.cx1, segment.cy1,
                segment.cx2, segment.cy2,
                segment.x1, segment.y1,
                arcLengths.at(segment).first, segmentCount);
            arcLengths.at(segment).second = std::accumulate(
                arcLengths.at(segment).first.begin(),
                arcLengths.at(segment).first.end(),
                0.0);
        }
    }

    BezierCoordinate BezierManager::transformIntoBezierCoordinate(
        const CubicBezier& segment,
        double x, double y, double z,
        const int segmentCount, const double segmentMultiplier)
    {
        computeBezierCoordinateSegments(segment, segmentCount);
        constexpr double SCARY_CONSTANT_THAT_I_DONT_WANNA_TOUCH = 0.46080857142;
        double t = x / arcLengths.at(segment).second * SCARY_CONSTANT_THAT_I_DONT_WANNA_TOUCH * static_cast<double>(segmentCount) * segmentMultiplier;
        CubicBezier segmentCopy = segment;

        if (t > 1)
        {
            double mrt = std::fmod(t, 1.0);
            double art = t - mrt;
            t = mrt;
            double tmpx1 = art * (segment.x1 - segment.x0);
            double tmpy1 = art * (segment.y1 - segment.y0);
            segmentCopy =
            {
                tmpx1 + segment.x0, tmpy1 + segment.y0,
                tmpx1 + segment.cx1, tmpy1 + segment.cy1,
                tmpx1 + segment.cx2, tmpy1 + segment.cy2,
                tmpx1 + segment.x1, tmpy1 + segment.y1,
            };
        }

        // First, we need to evaluate the Bezier curve for X and Z axis
        double bezierX = 0.0, bezierZ = 0.0, rotationY = 0.0;

        //// Evaluate the Bezier curve for X-axis using the segment's start and end points, and control points
        //evaluateCubicBezier(t, segment.x0, segment.y0, segment.cx1, segment.cy1, segment.cx2, segment.cy2, segment.x1, segment.y1,
        //    bezierX, bezierZ, rotationY);


        computeBezierCoordinateSegments(segmentCopy, segmentCount);

        // Evaluate the Bezier curve for X-axis using the segment's start and end points, and control points
        evaluateCubicBezierUniform(
            t,
            segmentCopy.x0, segmentCopy.y0,
            segmentCopy.cx1, segmentCopy.cy1,
            segmentCopy.cx2, segmentCopy.cy2,
            segmentCopy.x1, segmentCopy.y1,
            bezierX, bezierZ, rotationY,
            arcLengths.at(segmentCopy).first);


        // Return the transformed coordinates as a glm::vec3, with the original Y and Z coordinates being transformed along the curve
        return { glm::vec3(bezierX / segmentMultiplier, y, bezierZ / segmentMultiplier), glm::degrees(rotationY) };  // Since y is not involved in the Bezier curve transformation, it remains unchanged
    }
}