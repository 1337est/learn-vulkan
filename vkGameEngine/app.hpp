#pragma once

#include "vge_window.hpp"
#include "vge_pipeline.hpp"

namespace vge
{
class App
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

private:
    VgeWindow m_vgeWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    VgePipeline m_vgePipeline{"shaders/shader.vert.spv", "shaders/shader.frag.spv"};
};
}
