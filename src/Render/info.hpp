#pragma once

#include "render.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3>
    getAttributeDescriptions();
    bool operator==(const Vertex &other) const;
};

class RenderInformation {
  public:
    RenderInformation(std::string model_path);
    ~RenderInformation();

  private:
    friend Big::Renderer;
    static Big::Renderer *m_renderer;

    std::string m_model_path;
    int m_uuid;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};
