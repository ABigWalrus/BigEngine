#pragma once

// #include "render.hpp"
#include "../Engine/utility.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 tex_coord;

    static VkVertexInputBindingDescription get_binding_description();

    static std::array<VkVertexInputAttributeDescription, 3>
    get_attribute_descriptions();
    bool operator==(const Vertex &other) const;
};

class RenderInformation {
  public:
    RenderInformation(std::string model_path);
    ~RenderInformation();

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

  private:
    // friend Big::Renderer;
    // static Big::Renderer *m_renderer;

    std::string m_model_path;
    UUID m_uuid;

    void load_model();
};
