// GLFW creates windows, contexts, surfaces, receives input/events
#include <vulkan/vulkan_core.h>
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
    // glfw window
    GLFWwindow* window;

    // initialize GLFW and create a window
    void initWindow() {
        glfwInit(); // initialize GLFW library

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // stop OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable resize window

        // width, height, title, monitor, openglstuff
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    // vulkan instance
    VkInstance instance;

    // creating vulkan instance
    void createInstance() {
        // optional appInfo struct useful for optimizing app
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // tells vulkan driver which global extensions and layers we want
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Vulkan needs extensions to interface with the window system
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        // glfw gets extensions for us
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // pass the glfw values to the vulkan struct
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // global validation layers to enable
        createInfo.enabledLayerCount = 0;

        // Check if instance creation is successful or not with
        // - pointer to struct with creation info
        // - pointer  to custom allocator callbacks (nullptr here)
        // - pointer to variable that stores the handle to the new object
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void initVulkan() {
        createInstance();
    }

    void mainLoop() {
        // A loop that checks for window close events
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // cleanup instance
        vkDestroyInstance(instance, nullptr);

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
