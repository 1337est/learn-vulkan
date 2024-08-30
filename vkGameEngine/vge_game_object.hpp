#pragma once

// headers
#include "vge_model.hpp"

// libraries
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace vge
{
struct TransformComponent
{
    glm::vec3 translation{};          // position offset
    glm::vec3 scale{ 1.f, 1.f, 1.f }; // identity matrix
    glm::vec3 rotation{};

    // Matrix transformation corresponds to translate * Ry * Rx * Rz * scale
    // Rotation convention uses Tait-Bryan angles with axis order Y1, X2, Z3
    glm::mat4 mat4()
    {
        auto transform = glm::translate(glm::mat4{ 1.f }, translation);
        transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
        transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
        transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });
        transform = glm::scale(transform, scale);

        return transform;
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

    std::shared_ptr<VgeModel> m_model{};
    glm::vec3 m_color{};
    TransformComponent m_transform{};

private:
    VgeGameObject(id_t objId)
        : m_id{ objId }
    {
    }

    id_t m_id;
};
} // namespace vge
