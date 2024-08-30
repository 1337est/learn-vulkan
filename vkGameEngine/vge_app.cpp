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

// temporary helper function, creates a 1x1x1 cube centered at offset
std::unique_ptr<VgeModel> createCubeModel(VgeDevice& device, glm::vec3 offset)
{
    std::vector<VgeModel::Vertex> vertices{

        // left face (white)
        { { -.5f, -.5f, -.5f },  { .9f, .9f, .9f } },
        { { -.5f, .5f, .5f },    { .9f, .9f, .9f } },
        { { -.5f, -.5f, .5f },   { .9f, .9f, .9f } },
        { { -.5f, -.5f, -.5f },  { .9f, .9f, .9f } },
        { { -.5f, .5f, -.5f },   { .9f, .9f, .9f } },
        { { -.5f, .5f, .5f },    { .9f, .9f, .9f } },

        // right face (yellow)
        { { .5f, -.5f, -.5f },   { .8f, .8f, .1f } },
        { { .5f, .5f, .5f },     { .8f, .8f, .1f } },
        { { .5f, -.5f, .5f },    { .8f, .8f, .1f } },
        { { .5f, -.5f, -.5f },   { .8f, .8f, .1f } },
        { { .5f, .5f, -.5f },    { .8f, .8f, .1f } },
        { { .5f, .5f, .5f },     { .8f, .8f, .1f } },

        // top face (orange, remember y axis points down)
        { { -.5f, -.5f, -.5f },  { .9f, .6f, .1f } },
        { { .5f, -.5f, .5f },    { .9f, .6f, .1f } },
        { { -.5f, -.5f, .5f },   { .9f, .6f, .1f } },
        { { -.5f, -.5f, -.5f },  { .9f, .6f, .1f } },
        { { .5f, -.5f, -.5f },   { .9f, .6f, .1f } },
        { { .5f, -.5f, .5f },    { .9f, .6f, .1f } },

        // bottom face (red)
        { { -.5f, .5f, -.5f },   { .8f, .1f, .1f } },
        { { .5f, .5f, .5f },     { .8f, .1f, .1f } },
        { { -.5f, .5f, .5f },    { .8f, .1f, .1f } },
        { { -.5f, .5f, -.5f },   { .8f, .1f, .1f } },
        { { .5f, .5f, -.5f },    { .8f, .1f, .1f } },
        { { .5f, .5f, .5f },     { .8f, .1f, .1f } },

        // nose face (blue)
        { { -.5f, -.5f, 0.5f },  { .1f, .1f, .8f } },
        { { .5f, .5f, 0.5f },    { .1f, .1f, .8f } },
        { { -.5f, .5f, 0.5f },   { .1f, .1f, .8f } },
        { { -.5f, -.5f, 0.5f },  { .1f, .1f, .8f } },
        { { .5f, -.5f, 0.5f },   { .1f, .1f, .8f } },
        { { .5f, .5f, 0.5f },    { .1f, .1f, .8f } },

        // tail face (green)
        { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
        { { .5f, .5f, -0.5f },   { .1f, .8f, .1f } },
        { { -.5f, .5f, -0.5f },  { .1f, .8f, .1f } },
        { { -.5f, -.5f, -0.5f }, { .1f, .8f, .1f } },
        { { .5f, -.5f, -0.5f },  { .1f, .8f, .1f } },
        { { .5f, .5f, -0.5f },   { .1f, .8f, .1f } },
    };
    for (auto& v : vertices)
    {
        v.position += offset;
    }
    return std::make_unique<VgeModel>(device, vertices);
}

void VgeApp::loadGameObjects()
{
    std::shared_ptr<VgeModel> vgeModel =
        createCubeModel(m_vgeDevice, { .0f, .0f, .0f });

    auto cube = VgeGameObject::createGameObject();
    cube.m_model = vgeModel;
    cube.m_transform.translation = { .0f, .0f, .5f };
    cube.m_transform.scale = { .5f, .5f, .5f };
    m_gameObjects.push_back(std::move(cube));
}

} // namespace vge
