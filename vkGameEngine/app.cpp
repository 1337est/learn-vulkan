#include "app.hpp"

#include <GLFW/glfw3.h>

namespace vge
{
void App::run()
{
    while (!m_vgeWindow.shouldClose())
        glfwPollEvents();
}
}
