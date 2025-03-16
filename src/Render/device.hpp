#pragma once

#include "./window.hpp"

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace Big{
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

/**
 * Device related Extensions required for the app
*/
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
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

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

class Device{
public:
    Device(Window &window);
    ~Device();

    Device(const Device &) = delete;
    Device operator=(const Device &) = delete; 

    VkDevice getDevice();
    VkPhysicalDevice getPhysicalDevice();
    VkSurfaceKHR getVkSurface();
    VkSampleCountFlagBits getMsaaSamples();
    VkQueue getGraphicsQueue();
    VkQueue getPresentQueue();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
private:
    Window &window;

    void init();
    void cleanup();

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    void createInstance();
    void setupDebugMessenger();
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void createSurface();

    void pickPhysicalDevice();

    bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
    // QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    // SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSampleCountFlagBits getMaxUsableSampleCount();

    void createLogicalDevice();
};
}