#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vge
{

class VgeWindow
{
public:
    VgeWindow(int width, int height, std::string name); // constructor
    ~VgeWindow();                                       // destructor

    // Disable copy constructor and copy assignment operator= to prevent
    // undefined behavior with GLFWwindow* m_window pointer variable
    VgeWindow(const VgeWindow&) = delete;
    VgeWindow& operator=(const VgeWindow&) = delete;

    bool shouldClose()
    {
        return glfwWindowShouldClose(m_window);
    }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private: // member variables and functions
    GLFWwindow* m_window;
    const int m_width;
    const int m_height;
    std::string m_name;

    void initWindow();
};

} // namespace vge
