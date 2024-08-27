#pragma once

#include "vge_device.hpp"
#include "vge_pipeline.hpp"
#include "vge_swapchain.hpp"
#include "vge_window.hpp"

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
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    VgeWindow m_vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; //  window
    VgeDevice m_vgeDevice{ m_vgeWindow }; // use device for window
    VgeSwapChain m_vgeSwapChain{ m_vgeDevice, m_vgeWindow.getExtent() };
    std::unique_ptr<VgePipeline> m_vgePipeline;
    VkPipelineLayout m_pipelineLayout;
    std::vector<VkCommandBuffer> m_commandBuffers;
};

} // namespace vge
