#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vge {
class VgeWindow {
public: // constructor & destructor
    VgeWindow(int width, int height, std::string name);
    ~VgeWindow();

    VgeWindow(const VgeWindow&) = delete;
    VgeWindow& operator=(const VgeWindow&) = delete;

    bool shouldClose() { return glfwWindowShouldClose(m_window); }

private: // member variables and functions
    GLFWwindow *m_window;
    const int m_width;
    const int m_height;
    std::string m_name;

    void initWindow();
};
}
