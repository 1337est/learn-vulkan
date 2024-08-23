#pragma once

#include "vge_device.hpp"
#include "vge_pipeline.hpp"
#include "vge_window.hpp"

namespace vge
{

class App
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

private:
    VgeWindow m_vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; //  window
    VgeDevice m_vgeDevice{ m_vgeWindow }; // use device for window
    VgePipeline m_vgePipeline{ m_vgeDevice,
                               "shaders/shader.vert.spv", // vertex binary
                               "shaders/shader.frag.spv", // fragment binary
                               VgePipeline::defaultPipelineConfigInfo(WIDTH,
                                                                      HEIGHT) };
};

} // namespace vge
