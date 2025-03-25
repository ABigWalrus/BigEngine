#pragma once

#include <memory>
#include <unordered_map>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "device.hpp"
#include "info.hpp"
#include "window.hpp"

#include <optional>
#include <string>
#include <vector>

// struct Vertex {
//     glm::vec3 pos;
//     glm::vec3 color;
//     glm::vec2 texCoord;

//     static VkVertexInputBindingDescription getBindingDescription();

//     static std::array<VkVertexInputAttributeDescription, 3>
//     getAttributeDescriptions();
//     bool operator==(const Vertex &other) const;
// };
/**
 * Uniform Buffer Class
 *
 * Contains: Model Matrix, View Matrix, Projection Matrix
 *
 */
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

namespace Big {
/**
 * A Renderer class.
 *
 */
class Renderer {
  public:
    // GLFWwindow* window;

    // void init();
    void render();
    void cleanup();

    Renderer(std::shared_ptr<Big::Window> const &window,
             std::unique_ptr<Big::Device> const &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

  private:
    uint32_t m_width = 0;  // Window width
    uint32_t m_height = 0; // Window height

    uint32_t m_current_frame = 0; // The current frame: could be only mod values
                                  // of MAX_FRAMES_IN_FLIGHT

    bool m_framebuffer_resized = false;
    uint32_t m_mip_levels;

    // std::vector<Vertex> vertices;  // Array of vertices that go to vertex
    // buffer std::vector<uint32_t> indices; // Array of the corrsponding
    // indices

    std::shared_ptr<Big::Window> m_window;
    std::unique_ptr<Big::Device> m_device;

    VkSwapchainKHR m_swap_chain;

    std::vector<VkImage> m_swap_chain_images; // ToDo: try to use std::array

    VkFormat m_swap_chain_image_format;
    VkExtent2D m_swap_chain_extent;

    std::vector<VkImageView> m_swap_chain_image_views;
    VkRenderPass m_render_pass;
    VkDescriptorSetLayout m_descriptor_set_layout;
    VkPipelineLayout m_pipeline_layout;
    VkPipeline m_graphics_pipeline;

    std::vector<VkFramebuffer> m_swap_chain_framebuffers;

    VkCommandPool m_command_pool;

    VkImage m_color_image;
    VkDeviceMemory m_color_image_memory;
    VkImageView m_color_image_view;

    VkBuffer m_vertex_buffer;
    VkDeviceMemory m_vertex_buffer_memory;
    VkBuffer m_index_buffer;
    VkDeviceMemory m_index_buffer_memory;

    std::vector<VkBuffer> m_uniform_buffers;
    std::vector<VkDeviceMemory> m_uniform_buffers_memory;
    std::vector<void *> m_uniform_buffers_mapped;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage m_texture_image;
    VkDeviceMemory m_texture_image_memory;
    VkImageView m_texture_image_view;
    VkSampler m_texture_sampler;

    VkDescriptorPool m_descriptor_pool;
    std::vector<VkDescriptorSet> m_descriptor_sets;

    std::vector<VkCommandBuffer> m_command_buffers;

    std::vector<VkSemaphore> m_image_available_semaphores;
    std::vector<VkSemaphore> m_render_finished_semaphores;
    std::vector<VkFence> m_in_flight_fences;

    void init_vulkan();

    VkSurfaceFormatKHR choose_swap_surface_format(
        const std::vector<VkSurfaceFormatKHR> &available_formats);

    VkPresentModeKHR choose_swap_present_mode(
        const std::vector<VkPresentModeKHR> &available_present_modes);

    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities);

    void create_swap_chain();
    void create_image_views();
    void create_graphics_pipeline();

    VkShaderModule
    create_shader_module(const std::vector<char>
                             &code); // ToDo: use path to shader instead of code

    void create_render_pass();

    void create_framebuffers();

    void create_command_pool();

    void create_command_buffers();

    void record_command_buffer(VkCommandBuffer command_buffer,
                               uint32_t image_index);

    void create_sync_objects();

    void draw_frame();

    void recreate_swap_chain();

    void cleanup_swap_chain();

    static void framebuffer_resize_callback(GLFWwindow *window, int width,
                                            int height);

    void createVertexBuffer();

    uint32_t find_memory_type(uint32_t type_filter,
                              VkMemoryPropertyFlags properties);

    void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
                       VkMemoryPropertyFlags properties, VkBuffer &buffer,
                       VkDeviceMemory &buffer_memory);

    void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer,
                     VkDeviceSize size);

    void create_index_buffer();

    void create_descriptor_set_layout();

    void create_uniform_buffers();

    void update_uniform_buffer(uint32_t currentImage);

    void create_descriptor_pool();

    void create_descriptor_sets();

    void create_texture_image();

    void generate_mipmaps(VkImage image, VkFormat image_format,
                          int32_t tex_width, int32_t tex_height,
                          uint32_t mip_levels);

    void create_image(uint32_t width, uint32_t height, uint32_t mip_levels,
                      VkSampleCountFlagBits num_samples, VkFormat format,
                      VkImageTiling tiling, VkImageUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkImage &image,
                      VkDeviceMemory &image_memory);

    VkCommandBuffer begin_single_time_commands();

    void end_single_time_commands(VkCommandBuffer commandBuffer);
    void transition_image_layout(VkImage image, VkFormat format,
                                 VkImageLayout old_layout,
                                 VkImageLayout new_layout, uint32_t mip_levels);

    void copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width,
                              uint32_t height);

    void create_texture_image_view();

    VkImageView create_image_view(VkImage image, VkFormat format,
                                  VkImageAspectFlags aspect_flags,
                                  uint32_t mip_levels);

    void create_texture_sampler();

    void create_depth_resources();

    VkFormat find_supported_format(const std::vector<VkFormat> &candidates,
                                   VkImageTiling tiling,
                                   VkFormatFeatureFlags features);

    VkFormat find_depth_format();

    bool has_stencil_component(VkFormat format);

    void load_model(std::string model_path);

    void create_color_resources();
};
} // namespace Big