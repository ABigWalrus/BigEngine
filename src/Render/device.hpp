#pragma once

#include "./window.hpp"

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace Big {
const std::vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"};

/**
 * Device related Extensions required for the app
 */
const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif
struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool is_complete();
};

/**
 * The class contains details for the swap chain support
 *
 * Contents:
 * 1. Capabilities
 * 2. Formats
 * 3. PresentModes
 */
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

// VkResult s_create_debug_utils_messengerEXT(
//     VkInstance instance,
//     const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
//     const VkAllocationCallbacks *p_allocator,
//     VkDebugUtilsMessengerEXT *p_debug_messenger);
// void s_destroy_debug_utils_messengerEXT(
//     VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger,
//     const VkAllocationCallbacks *p_allocator);

class Device {
  public:
    Device(std::shared_ptr<Window> window);
    ~Device();

    Device(const Device &) = delete;
    Device operator=(const Device &) = delete;

    VkDevice get_device();
    VkPhysicalDevice get_physical_device();
    VkSurfaceKHR get_surface();
    VkSampleCountFlagBits get_msaa_samples();
    VkQueue get_graphics_queue();
    VkQueue get_present_queue();

    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device);
    QueueFamilyIndices find_queue_families(VkPhysicalDevice device);

  private:
    std::shared_ptr<Window> m_window;

    void init();
    void cleanup();

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debug_messenger;
    VkSurfaceKHR m_surface;

    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkSampleCountFlagBits m_msaa_samples = VK_SAMPLE_COUNT_1_BIT;

    VkDevice m_device;

    VkQueue m_graphics_queue;
    VkQueue m_present_queue;

    void create_instance();
    void setup_debug_messenger();
    bool check_validation_layer_support();
    std::vector<const char *> get_required_extensions();
    void populate_debug_messenger_create_info(
        VkDebugUtilsMessengerCreateInfoEXT &create_info);
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                   VkDebugUtilsMessageTypeFlagsEXT message_type,
                   const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
                   void *p_user_data);

    void create_surface();

    void pick_physical_device();

    bool is_physical_device_suitable(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool check_device_extension_support(VkPhysicalDevice device);
    // SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSampleCountFlagBits get_max_usable_sample_count();

    void create_logical_device();
};
} // namespace Big