#pragma once

// headers
#include "vge_device.hpp"
#include "vge_model.hpp"
#include "vge_pipeline.hpp"
#include "vge_swapchain.hpp"
#include "vge_window.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

// std
#include <memory>
#include <vector>

namespace vge
{

class App
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App();
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void run();

private:
    // Sierpinski exercise: draws recursive triangle
    void sierpinski(
        std::vector<VgeModel::Vertex>& vertices,
        int cuts,
        glm::vec2 a,
        glm::vec2 b,
        glm::vec2 c,
        glm::vec3 color);

    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(size_t imageIndex);

    VgeWindow m_vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; //  window
    VgeDevice m_vgeDevice{ m_vgeWindow }; // use device for window
    std::unique_ptr<VgeSwapChain> m_vgeSwapChain;
    std::unique_ptr<VgePipeline> m_vgePipeline;
    VkPipelineLayout m_pipelineLayout;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::unique_ptr<VgeModel> m_vgeModel;
};

} // namespace vge
