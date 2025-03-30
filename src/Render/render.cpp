#include "Render/device.hpp"
#include "Render/info.hpp"
#include "Render/window.hpp"
#include "glm/fwd.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
// #include <map>
#include <optional>
// #include <set>
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// #define TINYOBJLOADER_IMPLEMENTATION
// #include <tiny_obj_loader.h>

#include "render.hpp"

// const std::string MODEL_PATH = "../assets/models/viking_room.obj";
const std::string TEXTURE_PATH = "../assets/textures/viking_room.png";

const int MAX_FRAMES_IN_FLIGHT = 2;

static std::vector<char> read_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    size_t file_size = (size_t)file.tellg();
    std::vector<char> buffer(file_size);
    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();

    return buffer;
}

// VkVertexInputBindingDescription Vertex::getBindingDescription() {

//     VkVertexInputBindingDescription bindingDescription{};
//     bindingDescription.binding = 0;
//     bindingDescription.stride = sizeof(Vertex);
//     bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

//     return bindingDescription;
// }

// std::array<VkVertexInputAttributeDescription, 3>
// Vertex::getAttributeDescriptions() {
//     std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

//     attributeDescriptions[0].binding = 0;
//     attributeDescriptions[0].location = 0;
//     attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//     attributeDescriptions[0].offset = offsetof(Vertex, pos);

//     attributeDescriptions[1].binding = 0;
//     attributeDescriptions[1].location = 1;
//     attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//     attributeDescriptions[1].offset = offsetof(Vertex, color);

//     attributeDescriptions[2].binding = 0;
//     attributeDescriptions[2].location = 2;
//     attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
//     attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

//     return attributeDescriptions;
// }

// bool Vertex::operator==(const Vertex &other) const {
//     return pos == other.pos && color == other.color &&
//            texCoord == other.texCoord;
// }

// namespace std {
// template <> struct hash<Vertex> {
//     size_t operator()(Vertex const &vertex) const {
//         return ((hash<glm::vec3>()(vertex.pos) ^
//                  (hash<glm::vec3>()(vertex.color) << 1)) >>
//                 1) ^
//                (hash<glm::vec2>()(vertex.texCoord) << 1);
//     }
// };
// } // namespace std
// const std::vector<Vertex> vertices = {
//     {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//     {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//     {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//     {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

//     {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//     {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//     {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//     {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
// };

// const std::vector<uint16_t> indices = {
//     0, 1, 2, 2, 3, 0,
//     4, 5, 6, 6, 7, 4
// };
namespace Big {

Renderer::Renderer(std::shared_ptr<Window> window,
                   std::shared_ptr<Device> device,
                   std::shared_ptr<RenderInformation> render_info)
    : m_window{window}, m_device{device}, m_render_info{render_info} {
    m_width = m_window->get_width();
    m_height = m_window->get_height();
    // m_render_info{render_info};
    // RenderInformation::m_renderer = this;
    init_vulkan();
    m_window->set_framebuffer_size_callback(framebuffer_resize_callback);
}

// Renderer::Renderer(std::shared_ptr<Window>)

Renderer::~Renderer() { cleanup(); }

// void Renderer::add(int info) { std::cout << "puk"; }

void Renderer::init_vulkan() {
    create_swap_chain();
    create_image_views();
    create_render_pass();
    create_descriptor_set_layout();
    create_graphics_pipeline();
    create_command_pool();
    create_color_resources();
    create_depth_resources();
    create_framebuffers();
    create_texture_image();
    create_texture_image_view();
    create_texture_sampler();
    // load_model(MODEL_PATH);
    create_vertex_buffer();
    create_index_buffer();
    create_uniform_buffers();
    create_descriptor_pool();
    create_descriptor_sets();
    create_command_buffers();
    create_sync_objects();
}

void Renderer::render() { draw_frame(); }

void Renderer::cleanup() {
    VkDevice device = m_device->get_device();
    vkDeviceWaitIdle(device);
    std::cout << ". . . Cleaning up . . .\n";
    cleanup_swap_chain();

    vkDestroySampler(device, m_texture_sampler, nullptr);
    vkDestroyImageView(device, m_texture_image_view, nullptr);
    vkDestroyImage(device, m_texture_image, nullptr);
    vkFreeMemory(device, m_texture_image_memory, nullptr);

    vkDestroyPipeline(device, m_graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(device, m_pipeline_layout, nullptr);
    vkDestroyRenderPass(device, m_render_pass, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device, m_uniform_buffers[i], nullptr);
        vkFreeMemory(device, m_uniform_buffers_memory[i], nullptr);
    }

    vkDestroyDescriptorPool(device, m_descriptor_pool, nullptr);

    vkDestroyDescriptorSetLayout(device, m_descriptor_set_layout, nullptr);

    vkDestroyBuffer(device, m_index_buffer, nullptr);
    vkFreeMemory(device, m_index_buffer_memory, nullptr);

    vkDestroyBuffer(device, m_vertex_buffer, nullptr);
    vkFreeMemory(device, m_vertex_buffer_memory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, m_render_finished_semaphores[i], nullptr);
        vkDestroySemaphore(device, m_image_available_semaphores[i], nullptr);
        vkDestroyFence(device, m_in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(device, m_command_pool, nullptr);

    // vkDestroyDevice(device, nullptr);

    // if (enableValidationLayers) {
    //     DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    // }

    // vkDestroySurfaceKHR(instance, surface, nullptr);
    // vkDestroyInstance(instance, nullptr);

    // glfwDestroyWindow(window);

    // glfwTerminate();
}

/**
 * Here will be selected an image format and a color space of the surface
 * In this case we select:
 * - BRGA Image Format
 * - Nonlinear SRGB
 */
VkSurfaceFormatKHR Renderer::choose_swap_surface_format(
    const std::vector<VkSurfaceFormatKHR> &available_formats) {
    for (const auto &available_format : available_formats) {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return available_format;
        }
    }

    return available_formats[0];
}

/**
 * This function queries through Present Modes supported by device
 * Present Modes define how the images are displayed on the screen
 * - In our case we look for mailbox mode
 * - If not found returns FIFO mode (It is like vsync on)
 */
VkPresentModeKHR Renderer::choose_swap_present_mode(
    const std::vector<VkPresentModeKHR> &available_present_modes) {
    for (const auto &available_present_mode : available_present_modes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            std::cout << "MALIBOX PRESENT MODE AVAILABLE\n";
            return available_present_mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

/**
 * Here the function looks for the extent (proportions) for the screen
 * If not found returns the maximum screen (doesn't mean full screen)
 */
VkExtent2D
Renderer::choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        m_window->get_framebuffer_size(width, height);
        // glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                    static_cast<uint32_t>(height)};

        actual_extent.width =
            std::clamp(actual_extent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actual_extent.height =
            std::clamp(actual_extent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

        return actual_extent;
    }
}

/**
 * Creates the swap chain with:
 * - Surface Format
 * - Present Mode
 * - Swap Extent
 * - max possible image count
 */
void Renderer::create_swap_chain() {
    SwapChainSupportDetails swap_chain_support =
        m_device->query_swap_chain_support(m_device->get_physical_device());

    VkSurfaceFormatKHR surface_format =
        choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR present_mode =
        choose_swap_present_mode(swap_chain_support.presentModes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount;

    if (swap_chain_support.capabilities.maxImageCount > 0 &&
        image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = m_device->get_surface();

    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices =
        m_device->find_queue_families(m_device->get_physical_device());
    uint32_t queue_family_indices[] = {indices.graphics_family.value(),
                                       indices.present_family.value()};

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;     // Optional
        create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;

    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device->get_device(), &create_info, nullptr,
                             &m_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_device->get_device(), m_swap_chain, &image_count,
                            nullptr);
    m_swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_device->get_device(), m_swap_chain, &image_count,
                            m_swap_chain_images.data());

    m_swap_chain_image_format = surface_format.format;
    m_swap_chain_extent = extent;
}

void Renderer::create_image_views() {
    // swapChainImageViews.resize(swapChainImages.size());
    // for (size_t i = 0; i < swapChainImages.size(); i++) {
    //     VkImageViewCreateInfo createInfo{};
    //     createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    //     createInfo.image = swapChainImages[i];

    //     createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    //     createInfo.format = swapChainImageFormat;

    //     createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    //     createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    //     createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //     createInfo.subresourceRange.baseMipLevel = 0;
    //     createInfo.subresourceRange.levelCount = 1;
    //     createInfo.subresourceRange.baseArrayLayer = 0;
    //     createInfo.subresourceRange.layerCount = 1;

    //     if (vkCreateImageView(device, &createInfo, nullptr,
    //     &swapChainImageViews[i]) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create image views!");
    //     }
    // }

    m_swap_chain_image_views.resize(m_swap_chain_images.size());

    for (uint32_t i = 0; i < m_swap_chain_images.size(); i++) {
        m_swap_chain_image_views[i] =
            create_image_view(m_swap_chain_images[i], m_swap_chain_image_format,
                              VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }
}

/**
 * This function configures the graphics pipeline
 * - With vertex and fragment shader stages
 * - The description of bindings and attributes
 * - The geometric topology (in this case this is triangles)
 * - Rasterizer, viewport, multisampling, color blending, dynamic states, depth
 * stencil
 * - Pipeline layout
 */
void Renderer::create_graphics_pipeline() {
    auto vert_shader_code = read_file("../assets/shaders/vert.spv");
    auto frag_shader_code = read_file("../assets/shaders/frag.spv");

    VkShaderModule vert_shader_module = create_shader_module(vert_shader_code);
    VkShaderModule frag_shader_module = create_shader_module(frag_shader_code);

    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader_module;
    vert_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader_module;
    frag_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info,
                                                       frag_shader_stage_info};

    auto binding_description = Vertex::get_binding_description();
    auto attribute_descriptions = Vertex::get_attribute_descriptions();

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(
        attribute_descriptions.size()); // Here we pass the number of attributes
                                        // (position, color and uv)
    vertex_input_info.pVertexBindingDescriptions =
        &binding_description; // Optional
    vertex_input_info.pVertexAttributeDescriptions =
        attribute_descriptions.data(); // Optional

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    // VkViewport viewport{};
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = (float)swapChainExtent.width;
    // viewport.height = (float)swapChainExtent.height;
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;

    // VkRect2D scissor{};
    // scissor.offset = { 0, 0 };
    // scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f;          // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = m_device->get_msaa_samples();
    multisampling.minSampleShading = 0.2f;          // Optional
    multisampling.pSampleMask = nullptr;            // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;      // Optional

    VkPipelineColorBlendAttachmentState color_blend_attachment{};
    color_blend_attachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT,
                                                  VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamic_state{};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount =
        static_cast<uint32_t>(dynamic_states.size());
    dynamic_state.pDynamicStates = dynamic_states.data();

    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    depth_stencil.sType =
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;

    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;

    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.minDepthBounds = 0.0f; // Optional
    depth_stencil.maxDepthBounds = 1.0f; // Optional

    depth_stencil.stencilTestEnable = VK_FALSE;
    depth_stencil.front = {}; // Optional
    depth_stencil.back = {};  // Optional

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &m_descriptor_set_layout;
    pipeline_layout_info.pushConstantRangeCount = 0;    // Optional
    pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

    if (vkCreatePipelineLayout(m_device->get_device(), &pipeline_layout_info,
                               nullptr, &m_pipeline_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;

    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = &depth_stencil;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = &dynamic_state;

    pipeline_info.layout = m_pipeline_layout;

    pipeline_info.renderPass = m_render_pass;
    pipeline_info.subpass = 0;

    pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipeline_info.basePipelineIndex = -1;              // Optional

    if (vkCreateGraphicsPipelines(m_device->get_device(), VK_NULL_HANDLE, 1,
                                  &pipeline_info, nullptr,
                                  &m_graphics_pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(m_device->get_device(), frag_shader_module, nullptr);
    vkDestroyShaderModule(m_device->get_device(), vert_shader_module, nullptr);
}

VkShaderModule Renderer::create_shader_module(const std::vector<char> &code) {
    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shader_module;
    if (vkCreateShaderModule(m_device->get_device(), &create_info, nullptr,
                             &shader_module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shader_module;
}

void Renderer::create_render_pass() {
    VkAttachmentDescription color_attachment{};
    color_attachment.format = m_swap_chain_image_format;
    color_attachment.samples = m_device->get_msaa_samples();

    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment{};
    depth_attachment.format = find_depth_format();
    depth_attachment.samples = m_device->get_msaa_samples();
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref{};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription color_attachment_resolve{};
    color_attachment_resolve.format = m_swap_chain_image_format;
    color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_resolve_ref{};
    color_attachment_resolve_ref.attachment = 2;
    color_attachment_resolve_ref.layout =
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
    subpass.pResolveAttachments = &color_attachment_resolve_ref;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    // dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    // dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    // dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = {
        color_attachment, depth_attachment, color_attachment_resolve};

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount =
        static_cast<uint32_t>(attachments.size());
    render_pass_info.pAttachments = attachments.data();
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(m_device->get_device(), &render_pass_info, nullptr,
                           &m_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Renderer::create_framebuffers() {
    m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());

    for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
        std::array<VkImageView, 3> attachments = {m_color_image_view,
                                                  m_depth_image_view,
                                                  m_swap_chain_image_views[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_render_pass;
        framebufferInfo.attachmentCount =
            static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_swap_chain_extent.width;
        framebufferInfo.height = m_swap_chain_extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device->get_device(), &framebufferInfo,
                                nullptr,
                                &m_swap_chain_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Renderer::create_command_pool() {
    QueueFamilyIndices queue_family_indices =
        m_device->find_queue_families(m_device->get_physical_device());

    VkCommandPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    if (vkCreateCommandPool(m_device->get_device(), &pool_info, nullptr,
                            &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void Renderer::create_command_buffers() {
    m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = (uint32_t)m_command_buffers.size();

    if (vkAllocateCommandBuffers(m_device->get_device(), &alloc_info,
                                 m_command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::record_command_buffer(VkCommandBuffer command_buffer,
                                     uint32_t image_index) {
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;                  // Optional
    begin_info.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = m_render_pass;
    render_pass_info.framebuffer = m_swap_chain_framebuffers[image_index];

    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = m_swap_chain_extent;

    // VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    std::array<VkClearValue, 2> clear_values{};
    clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clear_values[1].depthStencil = {1.0f, 0};

    render_pass_info.clearValueCount =
        static_cast<uint32_t>(clear_values.size());
    render_pass_info.pClearValues = clear_values.data();

    vkCmdBeginRenderPass(command_buffer, &render_pass_info,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      m_graphics_pipeline);

    VkBuffer vertex_buffers[] = {m_vertex_buffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

    vkCmdBindIndexBuffer(command_buffer, m_index_buffer, 0,
                         VK_INDEX_TYPE_UINT32);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swap_chain_extent.width);
    viewport.height = static_cast<float>(m_swap_chain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swap_chain_extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    // vkCmdDraw(command_buffer, static_cast<uint32_t>(vertices.size()), 1, 0,
    // 0);
    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pipeline_layout, 0, 1,
                            &m_descriptor_sets[m_current_frame], 0, nullptr);
    vkCmdDrawIndexed(command_buffer,
                     static_cast<uint32_t>(m_render_info->m_indices.size()), 1,
                     0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Renderer::create_sync_objects() {
    m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(m_device->get_device(), &semaphore_info, nullptr,
                              &m_image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(m_device->get_device(), &semaphore_info, nullptr,
                              &m_render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device->get_device(), &fence_info, nullptr,
                          &m_in_flight_fences[i]) != VK_SUCCESS) {

            throw std::runtime_error(
                "failed to create synchronization objects for a frame!");
        }
    }
}

void Renderer::draw_frame() {
    vkWaitForFences(m_device->get_device(), 1,
                    &m_in_flight_fences[m_current_frame], VK_TRUE, UINT64_MAX);

    uint32_t image_index;
    VkResult result =
        vkAcquireNextImageKHR(m_device->get_device(), m_swap_chain, UINT64_MAX,
                              m_image_available_semaphores[m_current_frame],
                              VK_NULL_HANDLE, &image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreate_swap_chain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(m_device->get_device(), 1,
                  &m_in_flight_fences[m_current_frame]);

    vkResetCommandBuffer(m_command_buffers[m_current_frame], 0);
    record_command_buffer(m_command_buffers[m_current_frame], image_index);

    update_uniform_buffer(m_current_frame);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = {
        m_image_available_semaphores[m_current_frame]};
    VkPipelineStageFlags wait_stages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];

    VkSemaphore signal_semaphores[] = {
        m_render_finished_semaphores[m_current_frame]};
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(m_device->get_graphics_queue(), 1, &submit_info,
                      m_in_flight_fences[m_current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = {m_swap_chain};
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;

    present_info.pResults = nullptr;
    result = vkQueuePresentKHR(m_device->get_present_queue(), &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        m_framebuffer_resized) {
        m_framebuffer_resized = false;
        recreate_swap_chain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::recreate_swap_chain() {
    int width = 0, height = 0;
    m_window->get_framebuffer_size(width, height);
    while (width == 0 || height == 0) {
        m_window->get_framebuffer_size(width, height);
        // glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_device->get_device());

    cleanup_swap_chain();

    create_swap_chain();
    create_image_views();
    create_color_resources();
    create_depth_resources();
    create_framebuffers();
}

void Renderer::cleanup_swap_chain() {
    vkDestroyImageView(m_device->get_device(), m_depth_image_view, nullptr);
    vkDestroyImage(m_device->get_device(), m_depth_image, nullptr);
    vkFreeMemory(m_device->get_device(), m_depth_image_memory, nullptr);

    vkDestroyImageView(m_device->get_device(), m_color_image_view, nullptr);
    vkDestroyImage(m_device->get_device(), m_color_image, nullptr);
    vkFreeMemory(m_device->get_device(), m_color_image_memory, nullptr);

    for (size_t i = 0; i < m_swap_chain_framebuffers.size(); i++) {
        vkDestroyFramebuffer(m_device->get_device(),
                             m_swap_chain_framebuffers[i], nullptr);
    }

    for (size_t i = 0; i < m_swap_chain_image_views.size(); i++) {
        vkDestroyImageView(m_device->get_device(), m_swap_chain_image_views[i],
                           nullptr);
    }

    vkDestroySwapchainKHR(m_device->get_device(), m_swap_chain, nullptr);
}

void Renderer::framebuffer_resize_callback(GLFWwindow *window, int width,
                                           int height) {
    auto app = reinterpret_cast<Renderer *>(glfwGetWindowUserPointer(window));
    app->m_framebuffer_resized = true;
}

void Renderer::create_vertex_buffer() {
    // VkBufferCreateInfo bufferInfo{};
    // bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // bufferInfo.size = sizeof(vertices[0]) * vertices.size();

    // bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    // bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) !=
    // VK_SUCCESS) {
    //     throw std::runtime_error("failed to create vertex buffer!");
    // }

    // VkMemoryRequirements memRequirements;
    // vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    // VkMemoryAllocateInfo allocInfo{};
    // allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // allocInfo.allocationSize = memRequirements.size;
    // allocInfo.memoryTypeIndex =
    // findMemoryType(memRequirements.memoryTypeBits,
    // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) !=
    // VK_SUCCESS) {
    //     throw std::runtime_error("failed to allocate vertex buffer memory!");
    // }

    // vkBindBufferMemory(device, vertexBuffer, vertexBufferMemory, 0);

    VkDeviceSize buffer_size =
        sizeof(m_render_info->m_vertices[0]) * m_render_info->m_vertices.size();

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging_buffer, staging_buffer_memory);

    void *data;
    vkMapMemory(m_device->get_device(), staging_buffer_memory, 0, buffer_size,
                0, &data);
    memcpy(data, m_render_info->m_vertices.data(), (size_t)buffer_size);
    vkUnmapMemory(m_device->get_device(), staging_buffer_memory);
    create_buffer(buffer_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertex_buffer,
                  m_vertex_buffer_memory);
    copy_buffer(staging_buffer, m_vertex_buffer, buffer_size);

    vkDestroyBuffer(m_device->get_device(), staging_buffer, nullptr);
    vkFreeMemory(m_device->get_device(), staging_buffer_memory, nullptr);
}

uint32_t Renderer::find_memory_type(uint32_t typeFilter,
                                    VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(m_device->get_physical_device(),
                                        &mem_properties);

    for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags & properties) ==
                properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Renderer::create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkBuffer &buffer,
                             VkDeviceMemory &buffer_memory) {
    VkBufferCreateInfo buffer_info{};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_device->get_device(), &buffer_info, nullptr,
                       &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device->get_device(), buffer,
                                  &memRequirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = memRequirements.size;
    alloc_info.memoryTypeIndex =
        find_memory_type(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device->get_device(), &alloc_info, nullptr,
                         &buffer_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_device->get_device(), buffer, buffer_memory, 0);
}

void Renderer::copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer,
                           VkDeviceSize size) {
    // VkCommandBufferAllocateInfo allocInfo{};
    // allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    // allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    // allocInfo.commandPool = commandPool;
    // allocInfo.commandBufferCount = 1;

    // VkCommandBuffer commandBuffer;
    // vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // VkCommandBufferBeginInfo beginInfo{};
    // beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // VkBufferCopy copyRegion{};
    // copyRegion.srcOffset = 0; // Optional
    // copyRegion.dstOffset = 0; // Optional
    // copyRegion.size = size;
    // vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // vkEndCommandBuffer(commandBuffer);

    // VkSubmitInfo submitInfo{};
    // submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    // submitInfo.commandBufferCount = 1;
    // submitInfo.pCommandBuffers = &commandBuffer;

    // vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    // vkQueueWaitIdle(graphicsQueue);
    // vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkBufferCopy copy_region{};
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    end_single_time_commands(command_buffer);
}

void Renderer::create_index_buffer() {
    VkDeviceSize buffer_size =
        sizeof(m_render_info->m_indices[0]) * m_render_info->m_indices.size();

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging_buffer, staging_buffer_memory);

    void *data;
    vkMapMemory(m_device->get_device(), staging_buffer_memory, 0, buffer_size,
                0, &data);
    memcpy(data, m_render_info->m_indices.data(), (size_t)buffer_size);
    vkUnmapMemory(m_device->get_device(), staging_buffer_memory);

    create_buffer(buffer_size,
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_index_buffer,
                  m_index_buffer_memory);

    copy_buffer(staging_buffer, m_index_buffer, buffer_size);

    vkDestroyBuffer(m_device->get_device(), staging_buffer, nullptr);
    vkFreeMemory(m_device->get_device(), staging_buffer_memory, nullptr);
}

void Renderer::create_descriptor_set_layout() {

    VkDescriptorSetLayoutBinding ubo_layout_binding{};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_layout_binding.descriptorCount = 1;

    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutBinding sampler_layout_binding{};
    sampler_layout_binding.binding = 1;
    sampler_layout_binding.descriptorCount = 1;
    sampler_layout_binding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_layout_binding.pImmutableSamplers = nullptr;
    sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        ubo_layout_binding, sampler_layout_binding};

    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
    layout_info.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(m_device->get_device(), &layout_info,
                                    nullptr,
                                    &m_descriptor_set_layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void Renderer::create_uniform_buffers() {
    VkDeviceSize buffer_size = sizeof(UniformBufferObject);

    m_uniform_buffers.resize(MAX_FRAMES_IN_FLIGHT);
    m_uniform_buffers_memory.resize(MAX_FRAMES_IN_FLIGHT);
    m_uniform_buffers_mapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        create_buffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                      m_uniform_buffers[i], m_uniform_buffers_memory[i]);

        vkMapMemory(m_device->get_device(), m_uniform_buffers_memory[i], 0,
                    buffer_size, 0, &m_uniform_buffers_mapped[i]);
    }
}

void Renderer::update_uniform_buffer(uint32_t currentImage) {
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(
                     current_time - start_time)
                     .count();

    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    // ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
    // glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view =
        glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f),
                                m_swap_chain_extent.width /
                                    (float)m_swap_chain_extent.height,
                                0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(m_uniform_buffers_mapped[currentImage], &ubo, sizeof(ubo));
};

void Renderer::create_descriptor_pool() {
    std::array<VkDescriptorPoolSize, 2> pool_sizes{};
    pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_sizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pool_sizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo pool_info{};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
    pool_info.pPoolSizes = pool_sizes.data();
    pool_info.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(m_device->get_device(), &pool_info, nullptr,
                               &m_descriptor_pool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Renderer::create_descriptor_sets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
                                               m_descriptor_set_layout);
    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool;
    alloc_info.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    alloc_info.pSetLayouts = layouts.data();

    m_descriptor_sets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(m_device->get_device(), &alloc_info,
                                 m_descriptor_sets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo buffer_info{};
        buffer_info.buffer = m_uniform_buffers[i];
        buffer_info.offset = 0;
        buffer_info.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo image_info{};
        image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_info.imageView = m_texture_image_view;
        image_info.sampler = m_texture_sampler;

        // VkWriteDescriptorSet descriptorWrite{};
        // descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        // descriptorWrite.dstSet = descriptorSets[i];
        // descriptorWrite.dstBinding = 0;
        // descriptorWrite.dstArrayElement = 0;
        // descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        // descriptorWrite.descriptorCount = 1;

        // descriptorWrite.pBufferInfo = &buffer_info;
        // descriptorWrite.pImageInfo = nullptr; // Optional
        // descriptorWrite.pTexelBufferView = nullptr; // Optional

        std::array<VkWriteDescriptorSet, 2> descriptor_writes{};

        descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_writes[0].dstSet = m_descriptor_sets[i];
        descriptor_writes[0].dstBinding = 0;
        descriptor_writes[0].dstArrayElement = 0;
        descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_writes[0].descriptorCount = 1;
        descriptor_writes[0].pBufferInfo = &buffer_info;

        descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_writes[1].dstSet = m_descriptor_sets[i];
        descriptor_writes[1].dstBinding = 1;
        descriptor_writes[1].dstArrayElement = 0;
        descriptor_writes[1].descriptorType =
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptor_writes[1].descriptorCount = 1;
        descriptor_writes[1].pImageInfo = &image_info;

        vkUpdateDescriptorSets(m_device->get_device(),
                               static_cast<uint32_t>(descriptor_writes.size()),
                               descriptor_writes.data(), 0, nullptr);
    }
}

void Renderer::create_texture_image() {
    int tex_width, tex_height, tex_channels;
    stbi_uc *pixels = stbi_load(TEXTURE_PATH.c_str(), &tex_width, &tex_height,
                                &tex_channels, STBI_rgb_alpha);
    VkDeviceSize image_size = tex_width * tex_height * 4;

    m_mip_levels = static_cast<uint32_t>(
                       std::floor(std::log2(std::max(tex_width, tex_height)))) +
                   1;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;
    create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  staging_buffer, staging_buffer_memory);

    void *data;
    vkMapMemory(m_device->get_device(), staging_buffer_memory, 0, image_size, 0,
                &data);
    memcpy(data, pixels, static_cast<size_t>(image_size));
    vkUnmapMemory(m_device->get_device(), staging_buffer_memory);

    stbi_image_free(pixels);

    create_image(tex_width, tex_height, m_mip_levels, VK_SAMPLE_COUNT_1_BIT,
                 VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                 VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                     VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                     VK_IMAGE_USAGE_SAMPLED_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_texture_image,
                 m_texture_image_memory);

    transition_image_layout(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mip_levels);
    copy_buffer_to_image(staging_buffer, m_texture_image,
                         static_cast<uint32_t>(tex_width),
                         static_cast<uint32_t>(tex_height));
    // transitionImageLayout(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB,
    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
    vkDestroyBuffer(m_device->get_device(), staging_buffer, nullptr);
    vkFreeMemory(m_device->get_device(), staging_buffer_memory, nullptr);
    generate_mipmaps(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB, tex_width,
                     tex_height, m_mip_levels);
    // VkImageCreateInfo imageInfo{};
    // imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    // imageInfo.imageType = VK_IMAGE_TYPE_2D;
    // imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    // imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    // imageInfo.extent.depth = 1;
    // imageInfo.mipLevels = 1;
    // imageInfo.arrayLayers = 1;

    // imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    // imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    // imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    // VK_IMAGE_USAGE_SAMPLED_BIT;

    // imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    // imageInfo.flags = 0; // Optional

    // if (vkCreateImage(device, &imageInfo, nullptr, &textureImage) !=
    // VK_SUCCESS) {
    //     throw std::runtime_error("failed to create image!");
    // }

    // VkMemoryRequirements memRequirements;
    // vkGetImageMemoryRequirements(device, textureImage, &memRequirements);

    // VkMemoryAllocateInfo allocInfo{};
    // allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // allocInfo.allocationSize = memRequirements.size;
    // allocInfo.memoryTypeIndex =
    // findMemoryType(memRequirements.memoryTypeBits,
    // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    // if (vkAllocateMemory(device, &allocInfo, nullptr, &textureImageMemory) !=
    // VK_SUCCESS) {
    //     throw std::runtime_error("failed to allocate image memory!");
    // }

    // vkBindImageMemory(device, textureImage, textureImageMemory, 0);
}

void Renderer::generate_mipmaps(VkImage image, VkFormat image_format,
                                int32_t tex_width, int32_t tex_height,
                                uint32_t mip_levels) {

    VkFormatProperties format_properties;
    vkGetPhysicalDeviceFormatProperties(m_device->get_physical_device(),
                                        image_format, &format_properties);
    if (!(format_properties.optimalTilingFeatures &
          VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error(
            "texture image format does not support linear blitting!");
    }

    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mip_width = tex_width;
    int32_t mip_height = tex_height;

    for (uint32_t i = 1; i < mip_levels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);

        VkImageBlit blit{};
        blit.srcOffsets[0] = {0, 0, 0};
        blit.srcOffsets[1] = {mip_width, mip_height, 1};
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = {0, 0, 0};
        blit.dstOffsets[1] = {mip_width > 1 ? mip_width / 2 : 1,
                              mip_height > 1 ? mip_height / 2 : 1, 1};
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(
            command_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
                             nullptr, 0, nullptr, 1, &barrier);

        if (mip_width > 1)
            mip_width /= 2;
        if (mip_height > 1)
            mip_height /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mip_levels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                         0, nullptr, 1, &barrier);

    end_single_time_commands(command_buffer);
}

void Renderer::create_image(uint32_t width, uint32_t height,
                            uint32_t mip_levels,
                            VkSampleCountFlagBits num_samples, VkFormat format,
                            VkImageTiling tiling, VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties, VkImage &image,
                            VkDeviceMemory &image_memory) {
    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = mip_levels;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = num_samples;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(m_device->get_device(), &image_info, nullptr, &image) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements mem_requirements;
    vkGetImageMemoryRequirements(m_device->get_device(), image,
                                 &mem_requirements);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex =
        find_memory_type(mem_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device->get_device(), &alloc_info, nullptr,
                         &image_memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(m_device->get_device(), image, image_memory, 0);
}

VkCommandBuffer Renderer::begin_single_time_commands() {
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = m_command_pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(m_device->get_device(), &alloc_info,
                             &command_buffer);

    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    return command_buffer;
}

void Renderer::end_single_time_commands(VkCommandBuffer command_buffer) {
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(m_device->get_graphics_queue(), 1, &submit_info,
                  VK_NULL_HANDLE);
    vkQueueWaitIdle(m_device->get_graphics_queue());

    vkFreeCommandBuffers(m_device->get_device(), m_command_pool, 1,
                         &command_buffer);
}

void Renderer::transition_image_layout(VkImage image, VkFormat format,
                                       VkImageLayout old_layout,
                                       VkImageLayout new_layout,
                                       uint32_t mip_levels) {
    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mip_levels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    // barrier.srcAccessMask = 0; // TODO
    // barrier.dstAccessMask = 0; // TODO

    // vkCmdPipelineBarrier(
    //     commandBuffer,
    //     0 /* TODO */, 0 /* TODO */,
    //     0,
    //     0, nullptr,
    //     0, nullptr,
    //     1, &barrier
    // );

    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags destination_stage;

    // if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout ==
    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    //     barrier.srcAccessMask = 0;
    //     barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    //     source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    //     destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    // } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
    // new_layout
    // == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    //     barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    //     barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    //     source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    //     destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    // } else {
    //     throw std::invalid_argument("unsupported layout transition!");
    // }
    if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (has_stencil_component(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    } else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
        new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
               new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
               new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0,
                         nullptr, 0, nullptr, 1, &barrier);
    end_single_time_commands(command_buffer);
}

void Renderer::copy_buffer_to_image(VkBuffer buffer, VkImage image,
                                    uint32_t width, uint32_t height) {
    VkCommandBuffer command_buffer = begin_single_time_commands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(command_buffer, buffer, image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    end_single_time_commands(command_buffer);
}

void Renderer::create_texture_image_view() {
    // VkImageViewCreateInfo viewInfo{};
    // viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    // viewInfo.image = textureImage;
    // viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    // viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    // viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    // viewInfo.subresourceRange.baseMipLevel = 0;
    // viewInfo.subresourceRange.levelCount = 1;
    // viewInfo.subresourceRange.baseArrayLayer = 0;
    // viewInfo.subresourceRange.layerCount = 1;

    // if (vkCreateImageView(device, &viewInfo, nullptr, &textureImageView) !=
    // VK_SUCCESS) {
    //     throw std::runtime_error("failed to create texture image view!");
    // }

    m_texture_image_view =
        create_image_view(m_texture_image, VK_FORMAT_R8G8B8A8_SRGB,
                          VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels);
}

VkImageView Renderer::create_image_view(VkImage image, VkFormat format,
                                        VkImageAspectFlags aspect_flags,
                                        uint32_t mip_levels) {
    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    // view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.aspectMask = aspect_flags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = mip_levels;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    VkImageView image_view;
    if (vkCreateImageView(m_device->get_device(), &view_info, nullptr,
                          &image_view) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return image_view;
}

void Renderer::create_texture_sampler() {
    VkSamplerCreateInfo sampler_info{};
    sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.minFilter = VK_FILTER_LINEAR;

    sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(m_device->get_physical_device(), &properties);
    sampler_info.anisotropyEnable = VK_TRUE;
    sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    sampler_info.unnormalizedCoordinates = VK_FALSE;

    sampler_info.compareEnable = VK_FALSE;
    sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;

    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod = 0.0f;
    sampler_info.maxLod = static_cast<float>(m_mip_levels);

    if (vkCreateSampler(m_device->get_device(), &sampler_info, nullptr,
                        &m_texture_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void Renderer::create_depth_resources() {
    VkFormat depth_format = find_depth_format();
    create_image(m_swap_chain_extent.width, m_swap_chain_extent.height, 1,
                 m_device->get_msaa_samples(), depth_format,
                 VK_IMAGE_TILING_OPTIMAL,
                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_image,
                 m_depth_image_memory);
    m_depth_image_view = create_image_view(m_depth_image, depth_format,
                                           VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    transition_image_layout(
        m_depth_image, depth_format, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

VkFormat
Renderer::find_supported_format(const std::vector<VkFormat> &candidates,
                                VkImageTiling tiling,
                                VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_device->get_physical_device(),
                                            format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR &&
            (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                   (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat Renderer::find_depth_format() {
    return find_supported_format(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
         VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool Renderer::has_stencil_component(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// void Renderer::load_model(std::string model_path) {
//     tinyobj::attrib_t attrib;
//     std::vector<tinyobj::shape_t> shapes;
//     std::vector<tinyobj::material_t> materials;
//     std::string warn, err;

//     if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
//                           model_path.c_str())) {
//         throw std::runtime_error(warn + err);
//     }

//     std::unordered_map<Vertex, uint32_t> unique_vertices{};

//     for (const auto &shape : shapes) {
//         for (const auto &index : shape.mesh.indices) {
//             Vertex vertex{};

//             vertex.pos = {attrib.vertices[3 * index.vertex_index + 0],
//                           attrib.vertices[3 * index.vertex_index + 1],
//                           attrib.vertices[3 * index.vertex_index + 2]};

//             vertex.texCoord = {
//                 attrib.texcoords[2 * index.texcoord_index + 0],
//                 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

//             vertex.color = {1.0f, 1.0f, 1.0f};

//             if (unique_vertices.count(vertex) == 0) {
//                 unique_vertices[vertex] =
//                 static_cast<uint32_t>(vertices.size());
//                 vertices.push_back(vertex);
//             }

//             indices.push_back(unique_vertices[vertex]);
//         }
//     }
// }

// VkSampleCountFlagBits Renderer::getMaxUsableSampleCount() {
//     VkPhysicalDeviceProperties physicalDeviceProperties;
//     vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

//     VkSampleCountFlags counts =
//     physicalDeviceProperties.limits.framebufferColorSampleCounts &
//     physicalDeviceProperties.limits.framebufferDepthSampleCounts; if (counts
//     & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; } if (counts &
//     VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; } if (counts &
//     VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; } if (counts &
//     VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; } if (counts &
//     VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; } if (counts &
//     VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

//     return VK_SAMPLE_COUNT_1_BIT;
// }

void Renderer::create_color_resources() {
    VkFormat color_format = m_swap_chain_image_format;

    create_image(m_swap_chain_extent.width, m_swap_chain_extent.height, 1,
                 m_device->get_msaa_samples(), color_format,
                 VK_IMAGE_TILING_OPTIMAL,
                 VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_color_image,
                 m_color_image_memory);

    m_color_image_view = create_image_view(m_color_image, color_format,
                                           VK_IMAGE_ASPECT_COLOR_BIT, 1);
}
} // namespace Big
