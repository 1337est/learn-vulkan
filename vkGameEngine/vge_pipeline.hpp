#pragma once

// headers
#include "vge_device.hpp"
#include <vulkan/vulkan_core.h>

// std
#include <string>
#include <vector>

namespace vge
{

struct PipelineConfigInfo
{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;

    PipelineConfigInfo()
        : viewport{}
        , scissor{}
        , inputAssemblyInfo{}
        , rasterizationInfo{}
        , multisampleInfo{}
        , colorBlendAttachment{}
        , colorBlendInfo{}
        , depthStencilInfo{}
    {}
};

class VgePipeline
{
public:
    VgePipeline(
        VgeDevice& device,
        const std::string& vertFilepath,
        const std::string& fragFilePath,
        const PipelineConfigInfo& configInfo);
    ~VgePipeline();

    VgePipeline(const VgePipeline&) = delete;
    VgePipeline& operator=(const VgePipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static PipelineConfigInfo defaultPipelineConfigInfo(
        uint32_t width, uint32_t height);

private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilePath,
        const PipelineConfigInfo& configInfo);

    void createShaderModule(
        const std::vector<char>& code, VkShaderModule* shaderModule);

    VgeDevice& m_vgeDevice;
    VkPipeline m_graphicsPipeline;
    VkShaderModule m_vertShaderModule;
    VkShaderModule m_fragShaderModule;
};

} // namespace vge
