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
To draw an image from the swap chain, we wrap it into a `VkImageView` and `VkFramebuffer`. The image view references a specific part of an image to be used, and the frame buffers references image views that are to be used for color, depth, and stencil target. Since there's likely many differing images in the swap chain at a given moment, we'll preemptively create an image view and frame buffers for each of them and select the right one at draw time.

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
- Create frame buffers `VkFramebuffer` for the render pass
- Setup the graphics pipeline `VkPipeline`
- Allocate and record a command buffer `VkCommandBuffer` with the draw commands for every possible swap chain image
- Draw frames by acquiring images `vkAcquireNextImageKHR`, submitting the right draw command buffer `vkQueueSubmit`, and returning the images back to the swap chain `vkQueuPresentKHR`.

# Coding conventions

Vulkan functions have a lower case `vk` prefix, enums and structs have a `Vk` prefix, and enum values have a `VK_` prefix. The API heavily uses structs to provide parameters to functions.

# Triangle drawing steps, un-simplified
- GLFW for window creation with Vulkan
- Initialize Vulkan library with an instance
- Link GLFW extensions & Vulkan instance together
- Find a physical device/GPU
- Check the physical device/GPU
- Pick a physical device/GPU
- Find all the queue families for your chosen physical device/GPU
- Gather information for each queue family (operations supported, etc)
- Check if queue family supports what we want to do with the Vulkan API
- Add the queue check to the physical device/GPU check
- Create queue's we want from the physical device/GPU
- Specify the set of device features we'll be using
- Create the logical device info using the physical device and queue info
- Create the logical device using the physical device and logical device info
- Create a graphics queue handle to interface with the logical device and use the graphics card.
- Create a window surface with GLFW using the Vulkan instance and GLFW window (the surface is the canvas)
- Check the queue families you're using for window surface support (presentation queue)
- Create the presentation queue
- Create the presentation queue handle so we can draw to the window
- Check for swap chain support in device extensions
- Enable swap chain extensions through the logical device
- Check compatibility of our swap chain with our window surface
    - Check compatibility of basic surface capabilities (min/max # images in swap chain, min/max width/height of images)
    - Check compatibility of surface formats (pixel format, color space)
    - Check compatibility of available presentation modes
- Find and choose the optimum settings for the swap chain
    - Surface format (color depth)
    - Presentation mode (conditions for "swapping" images to the screen)
    - Swap extent (resolution of images in the swap chain)
- Create the swap chain info using the proper surface, surface format, presentation mode, swap extent, 
- Create the swap chain using the logical device and swap chain info
- Create a swap chain handle to retrieve the swap chain images
- Create the image view info to use the swap chain images
- Create the image view using the logical device and image view info
- Create a graphics pipeline function to use the image views.
- Create the shader files (vertex and fragment) for the graphics pipeline
- Compile the shader files
- Load the compiled shaders
- Create the shader modules info using the compiled shaders for the graphics pipeline
- Create the shader modules using the logical device and shader module info for the graphics pipeline
- Choose which stage of the graphics pipeline each shader modules will be used in (vert and frag)
- Create an array to hold the shader info structs.
- Create a dynamic state info for the graphics pipeline
- Create vertex input info for the graphics pipeline
- Create input assembly info for the graphics pipeline
- Initialize viewport information (view region)
- Initialize scissor information (crop region)
- Create the viewport info (view region)
- Create the scissor info (crop region)
- Create the rasterizer info from the vertex shader and turns it into fragments (pixels) to be colored by the fragment shader
- Create the multi sampling info by combining the fragment shader results of multiple polygons that rasterize to the same pixel, thus creating smoother images.
- Create the depth and stencil testing info, where depth determines whether a pixel should be drawn based on depth, and stencil testing determines where a pixel can be drawn
- Create the color blending state to combine the output color of the fragment shader with the color in the frame buffer.
- Create the color blending info
- Create the pipeline layout info (which is how resources are organized and accessed by the shaders)
- 
