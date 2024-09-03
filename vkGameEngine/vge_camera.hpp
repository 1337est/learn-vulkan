#pragma once

// libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vge
{
class VgeCamera
{
public:
    // x-axis is negative left & positive right
    // y-axis is negative up   & positive down
    // z-axis is negative near & positive far
    void setOrthographicProjection(
        float left,   // x: smaller
        float right,  // x: larger
        float top,    // y: smaller
        float bottom, // y: larger
        float near,   // z: smaller
        float far);   // z: larger

    void setPerspectiveProjection(
        float fovy,   // Vertical field of view
        float aspect, // Aspect ratio of window w/h
        float near,   // Near clipping plane
        float far);   // Far clipping plane

    const glm::mat4& getProjection() const
    {
        return m_projectionMatrix;
    }

private:
    glm::mat4 m_projectionMatrix{ 1.f };
};
} // namespace vge
