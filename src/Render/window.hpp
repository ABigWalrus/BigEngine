#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Big {
class Window {
  public:
    Window(std::string title, uint32_t width, uint32_t heigth);
    ~Window();

    Window(const Window &) = delete;
    Window operator=(const Window &) = delete;

    uint32_t get_width() const;
    uint32_t get_height() const;
    GLFWwindow *get_window() const;

    bool is_open() const;
    const char **get_required_windows_extensions(uint32_t &extension_count);
    VkResult create_window_vkSurface(VkInstance instance,
                                     const VkAllocationCallbacks *allocator,
                                     VkSurfaceKHR &surface);
    void set_framebuffer_size_callback(
        GLFWframebuffersizefun framebufferResizeCallback);
    void get_framebuffer_size(int &width, int &height);
    void window_wait_events();

  private:
    std::string title;

    const uint32_t width;
    const uint32_t heigth;

    GLFWwindow *window_pointer;

    void init_window();
    void cleanup_window();
};
} // namespace Big
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