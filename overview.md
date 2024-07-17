# Triangles drawing overview

No need to fully understand what's happening here. We simply just want to see how to steps correlate to one another to get a big picture view.

## **Step 1** - `VkInstance` & `VkPhysicalDevice`
- **`VkInstance`**: an object that initializes the Vulkan library for use.
- **`VkPhysicalDevice`**: A physical device (different from logical devices), selected to attach to the `VkInstance` object.

## **Step 2** - Logical device and queue families
- Once you select a hardware device in step 1 (physical device), you need to create a `VkDevice` (sounds like Vulkan Device), which I think is like the API logical device. This is where you'll describe specifically which `VkPhysicalDeviceFeatures` you'll be using, like multi viewport rendering and 64 bit floats.
- Most operations with Vulkan like, draw commands and memory operations are done asynchronously by submitting them to `VkQueue`.

## **Step 3** - Window surface and swap chain
You need a window to render images to. We'll use GLFW in this tutorial as the API for creating windows, contexts and surfaces, receiving inputs and events.

Two more components are necessary to render a window: a window surface `VkSurfaceKHR` and a swap chain `VkSwapchainKHR`. The `KHR` postfix means that these objects are part of a Vulkan extension.

## **Step 4** - Image views and framebuffers
To draw an image from the swap chain, we wrap it into a `VkImageView` and `VkFramebuffer`. The image view references a specific part of an image to be used, and the framebuffer references image views that are to be used for color, depth, and stencil target. Since there's likely many differing images in the swap chain at a given moment, we'll preemptively create an image view and framebuffer for each of them and select the right one at draw time.

## **Step 5** - Rander passes
Render passes in Vulkan describe the type of images used during rendering, how they will be used, and how their contents should be treated. For our initial triangle rendering application, we'll tell Vulkan that we want a single image as color target and that we want the image cleared to a solid color right before the drawing operation. `VkFramebuffer` actually binds specific images to these slots.

## **Step 6** - Graphics pipeline
The actual pipeline in Vulkan is setup by creating a `VkPipeline` object. This describes the configurable state of the graphics card, like viewport size and depth buffer operations and the programmable state using `VkShaderModule` objects, which are created from shader byte code.

## **Step 7** - Command pools and command buffers
Many of the operations Vulkan executes is submitted to a queue. These objects first need to be recorded into a `VkCommandBuffer`, which are allocated from a `VkCommandPool` that is associated with a specific queue family. To draw a triangle we need to record a command buffer with these operations:
1. Begin the render pass
2. Bind the graphics pipeline
3. Draw 3 vertices
4. End the render pass

## **Step 8** - Main loop
Now we acquire an image from the swap chain with `vkAcquireNextImageKHR`, select the appropriate command buffer for the image and execute it with `vkQueueSubmit`, and finally return the image to the swap chain for presentation to the screen with `vkQueuePresentKHR`.

# Triangle drawing steps simplified
- Create a `VkInstance`
- Select a supported graphics card `VkPhysicalDevice`
- Create a `VkDevice` and `VkQueue` for drawing and presentation
- Create a window, window surface, and swap chain
- Wrap the swap chain images into `VkImageView`
- Create a render pass that specifies the render targets and usage
- Create framebuffers `VkFramebuffer` for the render pass
- Setup the graphics pipeline `VkPipeline`
- Allocate and record a command buffer `VkCommandBuffer` with the draw commands for every possible swap chain image
- Draw frames by acquiring images `vkAcquireNextImageKHR`, submitting the right draw command buffer `vkQueueSubmit`, and returning the images back to the swap chain `vkQueuPresentKHR`.


