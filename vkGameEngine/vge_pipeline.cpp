#include "vge_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace vge {
VgePipeline::VgePipeline(const std::string &vertFilepath,
                         const std::string &fragFilePath) {
  createGraphicsPipeline(vertFilepath, fragFilePath);
}

std::vector<char> VgePipeline::readFile(const std::string &filepath) {
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

  file.close();
  return buffer;
}

void VgePipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                         const std::string &fragFilePath) {
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilePath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}
} // namespace vge
