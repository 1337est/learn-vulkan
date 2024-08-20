#include "vge_window.hpp"

#include <GLFW/glfw3.h>

namespace vge {
VgeWindow::VgeWindow(int width, int height, std::string name)
    : m_window{nullptr}, m_width{width}, m_height{height}, m_name(name) {
  initWindow();
}

VgeWindow::~VgeWindow() {
  glfwDestroyWindow(m_window); // Destroys our window context on exit
  glfwTerminate(); // free all remaining resources and uninitialize glfw
}

void VgeWindow::initWindow() {
  glfwInit();                                   // initialize glfw library
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Don't create OpenGL context
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);   // No resizeable windows

  // create the window
  m_window =
      glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);
}
} // namespace vge
