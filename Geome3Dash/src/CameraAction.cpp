#include "pch.h"
#include "../pch.h"

#include "CameraAction.h"

namespace g3d
{
    double easeInOutQuad(double t, double b, double c, double d)
    {
        t /= d / 2;
        if (t < 1) { return c / 2 * t * t + b; }
        t--;
        return -c / 2 * (t * (t - 2) - 1) + b;
    }

    glm::vec3 easeInOutQuad(double t, double d, const glm::vec3& origin, const glm::vec3& target)
    {
        glm::vec3 newVec3;
        newVec3.x = easeInOutQuad(t, origin.x, target.x - origin.x, d);
        newVec3.y = easeInOutQuad(t, origin.y, target.y - origin.y, d);
        newVec3.z = easeInOutQuad(t, origin.z, target.z - origin.z, d);
        return newVec3;
    }

    double CameraActionHandler::interpolate(double start, double end, double progress)
    {
        return easeInOutQuad(progress, start, end - start, 1.0);
    }

    void CameraActionHandler::update(double deltaTime, double currentXPosition,
        double& deltaX, double& deltaY, double& deltaZ,
        double& deltaYaw, double& deltaPitch)
    {
        if (currentActionIndex >= actions.size()) return;

        CameraAction& action = actions[currentActionIndex];

        // Check if the action should start based on the trigger
        if (currentXPosition < action.triggerAt) return;

        // Increment time for the current action
        currentTime += deltaTime;

        // Calculate interpolation progress
        double progress = currentTime / action.time;
        if (progress > 1.0) progress = 1.0; // Clamp progress to 1.0

        // Interpolate position and rotation
        double newX = interpolate(0, action.x, progress);
        double newY = interpolate(0, action.y, progress);
        double newZ = interpolate(0, action.z, progress);
        double newYaw = interpolate(0, action.yaw, progress);
        double newPitch = interpolate(0, action.pitch, progress);

        // Calculate deltas and output through references
        deltaX = newX - previousX;
        deltaY = newY - previousY;
        deltaZ = newZ - previousZ;
        deltaYaw = newYaw - previousYaw;
        deltaPitch = newPitch - previousPitch;

        // Update the previous values
        previousX = newX;
        previousY = newY;
        previousZ = newZ;
        previousYaw = newYaw;
        previousPitch = newPitch;

        // Move to the next action if done
        if (progress >= 1.0)
        {
            currentActionIndex++;
            currentTime = 0; // Reset time for the next action
            previousX = 0;
            previousY = 0;
            previousZ = 0;
            previousYaw = 0;
            previousPitch = 0;
        }
    }
}