// headers
#include "app.hpp"
#include "vge_pipeline.hpp"

// std
#include <array>
#include <memory>
#include <stdexcept>

namespace vge
{

App::App()
    : m_vgePipeline{}, m_pipelineLayout{}, m_commandBuffers{}, m_vgeModel{}
{
    loadModels();
    createPipelineLayout();
    createPipeline();
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
        sierpinski(vertices, cuts - 1, a, ab, ac, red);
        sierpinski(vertices, cuts - 1, ab, b, bc, green);
        sierpinski(vertices, cuts - 1, ac, bc, c, blue);
    }
}

void App::loadModels()
{
    std::vector<VgeModel::Vertex> vertices{};
    glm::vec3 vertColor{};
    sierpinski(
        vertices,
        1,
        { 0.0f, -0.5f },
        { 0.5f, 0.5f },
        { -0.5f, 0.5f },
        vertColor);
    m_vgeModel = std::make_unique<VgeModel>(m_vgeDevice, vertices);
}

/*
void App::loadModels()
{
    std::vector<VgeModel::Vertex> vertices{
        { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.5f, 0.5f },  { 0.0f, 1.0f, 0.0f } },
        { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } }
    };
    m_vgeModel = std::make_unique<VgeModel>(m_vgeDevice, vertices);
}
*/

void App::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
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
    auto pipelineConfig = VgePipeline::defaultPipelineConfigInfo(
        m_vgeSwapChain.width(), m_vgeSwapChain.height());
    pipelineConfig.renderPass = m_vgeSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = m_pipelineLayout;
    m_vgePipeline = std::make_unique<VgePipeline>(
        m_vgeDevice,
        "./shaders/shader.vert.spv",
        "./shaders/shader.frag.spv",
        pipelineConfig);
}

void App::createCommandBuffers()
{
    m_commandBuffers.resize(m_vgeSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_vgeDevice.getCommandPool();
    allocInfo.commandBufferCount =
        static_cast<uint32_t>(m_commandBuffers.size());

    if (vkAllocateCommandBuffers(
            m_vgeDevice.device(), &allocInfo, m_commandBuffers.data()) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffersf!");
    }

    for (size_t i = 0; i < m_commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error(
                "Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_vgeSwapChain.getRenderPass();
        renderPassInfo.framebuffer = m_vgeSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_vgeSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f }; // index 0 is color
        clearValues[1].depthStencil = { 1.0f, 0 };         // index 1 is depth
        renderPassInfo.clearValueCount =
            static_cast<uint32_t>(clearValues.size());

        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(
            m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_vgePipeline->bind(m_commandBuffers[i]);
        m_vgeModel->bind(m_commandBuffers[i]);
        m_vgeModel->draw(m_commandBuffers[i]);

        vkCmdEndRenderPass(m_commandBuffers[i]);
        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void App::drawFrame()
{
    uint32_t imageIndex;
    auto result = m_vgeSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image!");
    }

    result = m_vgeSwapChain.submitCommandBuffers(
        &m_commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image!");
    }
}

} // namespace vge
