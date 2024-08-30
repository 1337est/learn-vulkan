// headers
#include "app.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

// libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace vge
{

struct SimplePushConstantData
{
    glm::mat2 transform{ 1.f }; // identity matrix
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

App::App()
    : m_vgePipeline{}
    , m_pipelineLayout{}
    , m_gameObjects{}
{
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
}

App::~App()
{
    vkDestroyPipelineLayout(m_vgeDevice.device(), m_pipelineLayout, nullptr);
}

void App::run()
{
    // run until window closes
    while (!m_vgeWindow.shouldClose())
    {
        glfwPollEvents(); // continuously processes and returns received events

        // beginFrame returns nullptr if swapchain needs to be recreated
        if (auto commandBuffer = m_vgeRenderer.beginFrame())
        {
            m_vgeRenderer.beginSwapChainRenderPass(commandBuffer);
            renderGameObjects(commandBuffer);
            m_vgeRenderer.endSwapChainRenderPass(commandBuffer);
            m_vgeRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(m_vgeDevice.device());
}

void App::loadGameObjects()
{
    std::vector<VgeModel::Vertex> vertices{
        { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.5f, 0.5f },  { 0.0f, 1.0f, 0.0f } },
        { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
    };
    auto vgeModel = std::make_shared<VgeModel>(m_vgeDevice, vertices);

    auto triangle = VgeGameObject::createGameObject();
    triangle.model = vgeModel;
    triangle.color = { 0.1f, 0.8f, 0.1f };
    triangle.transform2D.translation.x = 0.2f;
    triangle.transform2D.scale = { 2.f, .5f };
    triangle.transform2D.rotation = 0.25f * glm::two_pi<float>(); // 2pi=1rot

    m_gameObjects.push_back(std::move(triangle));
}

void App::createPipelineLayout()
{
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags =
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(
            m_vgeDevice.device(),
            &pipelineLayoutInfo,
            nullptr,
            &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout!");
    }
}

void App::createPipeline()
{
    assert(
        m_pipelineLayout != nullptr &&
        "Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig{};
    VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = m_vgeRenderer.getSwapChainRenderPass();
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_vgePipeline = std::make_unique<VgePipeline>(
        m_vgeDevice,
        "./shaders/shader.vert.spv",
        "./shaders/shader.frag.spv",
        pipelineConfig);
}

void App::renderGameObjects(VkCommandBuffer commandBuffer)
{
    m_vgePipeline->bind(commandBuffer);

    for (auto& obj : m_gameObjects)
    {
        obj.transform2D.rotation =
            glm::mod(obj.transform2D.rotation + 0.0001f, glm::two_pi<float>());

        SimplePushConstantData pushData{};
        pushData.offset = obj.transform2D.translation;
        pushData.color = obj.color;
        pushData.transform = obj.transform2D.mat2();

        vkCmdPushConstants(
            commandBuffer,
            m_pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &pushData);
        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}

} // namespace vge

/*
// Sierpinski exercise: draws recursive triangle
void App::sierpinski(
    std::vector<VgeModel::Vertex>& vertices,
    int cuts,
    glm::vec2 a,
    glm::vec2 b,
    glm::vec2 c,
    [[maybe_unused]] glm::vec3 RGBColor)
{
    glm::vec3 red = { 1.0f, 0.0f, 0.0f };
    glm::vec3 green = { 0.0f, 1.0f, 0.0f };
    glm::vec3 blue = { 0.0f, 0.0f, 1.0f };

    if (cuts <= 0)
    {
        vertices.push_back({ a, RGBColor });
        vertices.push_back({ b, RGBColor });
        vertices.push_back({ c, RGBColor });
    }
    else
    {
        auto ab = 0.5f * (a + b);
        auto ac = 0.5f * (a + c);
        auto bc = 0.5f * (b + c);
        // colored like the LOZ triforce (colors counterclockwise)
        sierpinski(vertices, cuts - 1, a, ab, ac, red);   // top triangle
        sierpinski(vertices, cuts - 1, ab, b, bc, green); // right triangle
        sierpinski(vertices, cuts - 1, ac, bc, c, blue);  // left triangle
    }
}

void App::loadModels()
{
    std::vector<VgeModel::Vertex> vertices{};
    glm::vec3 vertColor{};
    sierpinski(
        vertices,
        2,
        { 0.0f, -0.5f },
        { 0.5f, 0.5f },
        { -0.5f, 0.5f },
        vertColor);
    m_vgeModel = std::make_unique<VgeModel>(m_vgeDevice, vertices);
}
*/
