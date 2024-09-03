#pragma once

// headers
#include "vge_device.hpp"

// libraries
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace vge
{

class VgeModel
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription>
        getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription>
        getAttributeDescriptions();
    };

    struct Builder
    {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };

    VgeModel(VgeDevice& device, const VgeModel::Builder& builder);
    ~VgeModel();

    VgeModel(const VgeModel&) = delete;
    VgeModel& operator=(const VgeModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    VgeDevice& m_vgeDevice;

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    uint32_t m_vertexCount;

    bool m_hasIndexBuffer = false;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    uint32_t m_indexCount;
};

} // namespace vge
