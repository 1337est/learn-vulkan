#pragma once

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
    VgeWindow m_vgeWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; // test
    VgePipeline m_vgePipeline{ "shaders/shader.vert.spv",    // test
                               "shaders/shader.frag.spv" };  // test
};
} // namespace vge
