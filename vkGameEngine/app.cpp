#include "app.hpp"

#include <GLFW/glfw3.h>

namespace vge
{

void App::run()
{
    // run until window closes
    while (!m_vgeWindow.shouldClose())
        glfwPollEvents(); // continuously processes and returns received events
}

} // namespace vge
