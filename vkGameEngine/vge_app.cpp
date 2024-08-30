// headers
#include "vge_app.hpp"
#include "vge_render_system.hpp"

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

VgeApp::VgeApp()
    : m_gameObjects{}
{
    loadGameObjects();
}

VgeApp::~VgeApp()
{
}

void VgeApp::run()
{
    VgeRenderSystem renderSystem{ m_vgeDevice,
                                  m_vgeRenderer.getSwapChainRenderPass() };
    // run until window closes
    while (!m_vgeWindow.shouldClose())
    {
        glfwPollEvents(); // continuously processes and returns received events

        // beginFrame returns nullptr if swapchain needs to be recreated
        if (auto commandBuffer = m_vgeRenderer.beginFrame())
        {
            m_vgeRenderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem.renderGameObjects(commandBuffer, m_gameObjects);
            m_vgeRenderer.endSwapChainRenderPass(commandBuffer);
            m_vgeRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(m_vgeDevice.device());
}

void VgeApp::loadGameObjects()
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

} // namespace vge
