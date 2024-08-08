// GLFW creates windows, contexts, surfaces, receives input/events
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;

    // initialize GLFW and create a window
    void initWindow() {
        glfwInit(); // initialize GLFW library

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // stop OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resize window

        // width, height, title, monitor, openglstuff
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {

    }

    void mainLoop() {
        // A loop that checks for window close events
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // Cleanup after window exits
        glfwDestroyWindow(window);

        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
