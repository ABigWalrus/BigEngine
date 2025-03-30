#include "window.hpp"

namespace Big {

Window::Window(std::string title, uint32_t width, uint32_t heigth)
    : m_title{title}, width{width}, heigth{heigth} {
    init_window();
}

Window::~Window() { cleanup_window(); }

void Window::init_window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_pointer =
        glfwCreateWindow(width, heigth, m_title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window_pointer, this);
}

void Window::cleanup_window() {
    glfwDestroyWindow(window_pointer);
    glfwTerminate();
}

const char **
Window::get_required_windows_extensions(uint32_t &extension_count) {
    return glfwGetRequiredInstanceExtensions(&extension_count);
}

VkResult Window::create_window_vkSurface(VkInstance instance,
                                         const VkAllocationCallbacks *allocator,
                                         VkSurfaceKHR &surface) {
    return glfwCreateWindowSurface(instance, window_pointer, allocator,
                                   &surface);
}

void Window::set_framebuffer_size_callback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(window_pointer, callback);
}

uint32_t Window::get_width() const { return width; }

uint32_t Window::get_height() const { return heigth; }

GLFWwindow *Window::get_window() const { return window_pointer; }

bool Window::is_open() const { return !glfwWindowShouldClose(window_pointer); }

void Window::get_framebuffer_size(int &width, int &height) {
    glfwGetFramebufferSize(window_pointer, &width, &height);
}

void Window::window_wait_events() { glfwWaitEvents(); }
} // namespace Big