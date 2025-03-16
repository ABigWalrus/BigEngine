#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Big{
    class Window{
    public:
        Window(std::string windowName, uint32_t width, uint32_t heigth);
        ~Window();

        Window(const Window &) = delete;
        Window operator=(const Window &) = delete; 

        uint32_t getWidth() const;
        uint32_t getHeight() const;
        GLFWwindow* getWindow() const;
        
        bool isOpen();
        const char** getRequiredWindowsExtensions(uint32_t &extensionCount);
        VkResult createWindowVkSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR &surface);
        void setFramebufferSizeCallback(GLFWframebuffersizefun framebufferResizeCallback);
        void getFramebufferSize(int &width, int &height);
        void windowWaitEvents();
    private:
        std::string windowName;
        
        const uint32_t width;
        const uint32_t heigth;

        GLFWwindow* windowPointer;

        void initWindow();
        void cleanupWindow();
    };
}
// class BigWindow{
// public:
//     BigWindow(std::string windowName, uint32_t width, uint32_t heigth);
//     ~BigWindow();
//     uint32_t getWidth() const;
//     uint32_t getHeight() const;
//     GLFWwindow* getWindow() const;
    
//     bool isOpen();
// private:
//     std::string windowName;
    
//     const uint32_t width;
//     const uint32_t heigth;

//     GLFWwindow* windowPointer;

//     void initWindow();
//     void cleanupWindow();
// };