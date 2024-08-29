// headers
#include "app.hpp"
#include <vulkan/vulkan_core.h>

// libraries
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace vge
{

struct SimplePushConstantData
{
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

App::App()
    : m_vgeSwapChain{}
    , m_vgePipeline{}
    , m_pipelineLayout{}
    , m_commandBuffers{}
    , m_vgeModel{}
{
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
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
        drawFrame();
    }

    vkDeviceWaitIdle(m_vgeDevice.device());
}

void App::loadModels()
{
    std::vector<VgeModel::Vertex> vertices{
        { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.5f, 0.5f },  { 0.0f, 1.0f, 0.0f } },
        { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
    };
    m_vgeModel = std::make_unique<VgeModel>(m_vgeDevice, vertices);
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
        m_vgeSwapChain != nullptr &&
        "Cannot create pipeline before swapchain!");
    assert(
        m_pipelineLayout != nullptr &&
        "Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig{};
    VgePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = m_vgeSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_vgePipeline = std::make_unique<VgePipeline>(
        m_vgeDevice,
        "./shaders/shader.vert.spv",
        "./shaders/shader.frag.spv",
        pipelineConfig);
}

void App::recreateSwapChain()
{
    auto extent = m_vgeWindow.getExtent();
    while (extent.width == 0 || extent.height == 0)
    {
        extent = m_vgeWindow.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(m_vgeDevice.device());

    if (m_vgeSwapChain == nullptr)
    {
        m_vgeSwapChain = std::make_unique<VgeSwapChain>(m_vgeDevice, extent);
    }
    else
    {
        m_vgeSwapChain = std::make_unique<VgeSwapChain>(
            m_vgeDevice,
            extent,
            std::move(m_vgeSwapChain));
        if (m_vgeSwapChain->imageCount() != m_commandBuffers.size())
        {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }

    // TODO: If render pass is compatible, do nothing else
    createPipeline();
}

void App::createCommandBuffers()
{
    m_commandBuffers.resize(m_vgeSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vgeDevice.getCommandPool();
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(m_commandBuffers.size());

    if (vkAllocateCommandBuffers(
            m_vgeDevice.device(),
            &allocInfo,
            m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffersf!");
    }
}

void App::freeCommandBuffers()
{
    vkFreeCommandBuffers(
        m_vgeDevice.device(),
        m_vgeDevice.getCommandPool(),
        static_cast<uint32_t>(m_commandBuffers.size()),
        m_commandBuffers.data());

    m_commandBuffers.clear();
}

void App::recordCommandBuffer(size_t imageIndex)
{
    static int frame = 0;
    frame = (frame + 1) % 10'000;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_commandBuffers[imageIndex], &beginInfo) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_vgeSwapChain->getRenderPass();
    renderPassInfo.framebuffer = m_vgeSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = m_vgeSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f }; // index 0 is color
    clearValues[1].depthStencil = { 1.0f, 0 };            // index 1 is depth
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());

    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(
        m_commandBuffers[imageIndex],
        &renderPassInfo,
        VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width =
        static_cast<float>(m_vgeSwapChain->getSwapChainExtent().width);
    viewport.height =
        static_cast<float>(m_vgeSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{
        { 0, 0 },
        m_vgeSwapChain->getSwapChainExtent()
    };
    vkCmdSetViewport(m_commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(m_commandBuffers[imageIndex], 0, 1, &scissor);

    m_vgePipeline->bind(m_commandBuffers[imageIndex]);
    m_vgeModel->bind(m_commandBuffers[imageIndex]);

    for (int j = 0; j < 4; j++)
    {
        SimplePushConstantData pushData{};
        pushData.offset = { -0.5f + static_cast<float>(frame) * 0.0002f,
                            -0.4f + static_cast<float>(j) * 0.25f };
        pushData.color = { 0.0f, 0.0f, 0.2f + 0.2f * static_cast<float>(j) };

        vkCmdPushConstants(
            m_commandBuffers[imageIndex],
            m_pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &pushData);
        m_vgeModel->draw(m_commandBuffers[imageIndex]);
    }

    vkCmdEndRenderPass(m_commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(m_commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void App::drawFrame()
{
    uint32_t imageIndex;
    auto result = m_vgeSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = m_vgeSwapChain->submitCommandBuffers(
        &m_commandBuffers[imageIndex],
        &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        m_vgeWindow.wasWindowResized())
    {
        m_vgeWindow.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image!");
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
