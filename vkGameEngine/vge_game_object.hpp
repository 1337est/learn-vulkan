#pragma once

// headers
#include "vge_model.hpp"

// std
#include <memory>

namespace vge
{
struct Transform2DComponent
{
    glm::vec2 translation{}; // position offset
    glm::vec2 scale{ 1.f, 1.f };
    float rotation;

    glm::mat2 mat2()
    {
        // for vector rotations
        const float sinTheta = glm::sin(rotation);
        const float cosTheta = glm::cos(rotation);
        glm::mat2 rotMatrix{
            { cosTheta,  sinTheta },
            { -sinTheta, cosTheta }
        };
        // for scalar multiplication
        glm::mat2 scaleMatrix{
            { scale.x, .0f     },
            { .0f,     scale.y }
        };
        return rotMatrix * scaleMatrix; // identity matrix
    }
};

class VgeGameObject

{
public:
    using id_t = unsigned int;

    static VgeGameObject createGameObject()
    {
        static id_t currentId = 0;
        return VgeGameObject{ currentId++ };
    }

    VgeGameObject(const VgeGameObject&) = delete;
    VgeGameObject& operator=(const VgeGameObject&) = delete;
    VgeGameObject(VgeGameObject&&) = default;
    VgeGameObject& operator=(VgeGameObject&&) = default;

    id_t getId()
    {
        return m_id;
    }

    std::shared_ptr<VgeModel> model{};
    glm::vec3 color{};
    Transform2DComponent transform2D{};

private:
    VgeGameObject(id_t objId)
        : m_id{ objId }
    {
    }

    id_t m_id;
};
} // namespace vge
