#pragma once

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
    void add(RenderInformation info);

    Renderer(Big::Window &window, Big::Device &device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

  private:
    uint32_t WIDTH = 0;  // Window width
    uint32_t HEIGHT = 0; // Window height

    uint32_t currentFrame = 0; // The current frame: could be only mod values of
                               // MAX_FRAMES_IN_FLIGHT

    bool framebufferResized = false;
    uint32_t mipLevels;

    // std::vector<Vertex> vertices;  // Array of vertices that go to vertex
    // buffer std::vector<uint32_t> indices; // Array of the corrsponding
    // indices

    Big::Window &bigWindow;
    Big::Device &bigDevice;

    std::unordered_map<int, RenderInformation> m_info_map;

    VkSwapchainKHR swapChain;

    std::vector<VkImage> swapChainImages;

    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    void initVulkan();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void createSwapChain();
    void createImageViews();
    void createGraphicsPipeline();

    VkShaderModule createShaderModule(const std::vector<char> &code);

    void createRenderPass();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex);

    void createSyncObjects();

    void drawFrame();

    void recreateSwapChain();

    void cleanupSwapChain();

    static void framebufferResizeCallback(GLFWwindow *window, int width,
                                          int height);

    void createVertexBuffer();

    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createIndexBuffer();

    void createDescriptorSetLayout();

    void createUniformBuffers();

    void updateUniformBuffer(uint32_t currentImage);

    void createDescriptorPool();

    void createDescriptorSets();

    void createTextureImage();

    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth,
                         int32_t texHeight, uint32_t mipLevels);

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
                     VkSampleCountFlagBits numSamples, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &imageMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void transitionImageLayout(VkImage image, VkFormat format,
                               VkImageLayout oldLayout, VkImageLayout newLayout,
                               uint32_t mipLevels);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                           uint32_t height);

    void createTextureImageView();

    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags,
                                uint32_t mipLevels);

    void createTextureSampler();

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void loadModel(std::string model_path);

    void createColorResources();
};
} // namespace Big