#include "window.hpp"

#include <string>

namespace Big{


Window::Window(std::string _windowName, uint32_t _width, uint32_t _heigth):windowName{_windowName}, width{_width}, heigth{_heigth}{
    initWindow();
}

Window::~Window(){
    cleanupWindow();
}

void Window::initWindow(){
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    windowPointer = glfwCreateWindow(width, heigth, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(windowPointer, this);
}

void Window::cleanupWindow(){
    glfwDestroyWindow(windowPointer);
    glfwTerminate();
}

const char** Window::getRequiredWindowsExtensions(uint32_t &extensionCount){
    return glfwGetRequiredInstanceExtensions(&extensionCount);
}

VkResult Window::createWindowVkSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR &surface){
    return glfwCreateWindowSurface(instance, windowPointer, allocator, &surface);
}

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback){
    glfwSetFramebufferSizeCallback(windowPointer, callback);
}

uint32_t Window::getWidth() const{
    return width;
}

uint32_t Window::getHeight() const{
    return heigth;
}

GLFWwindow* Window::getWindow() const{
    return windowPointer;
}

bool Window::isOpen(){
        return !glfwWindowShouldClose(windowPointer);
}

void Window::getFramebufferSize(int &width, int &height){
    glfwGetFramebufferSize(windowPointer, &width, &height);
}

void Window::windowWaitEvents(){
    glfwWaitEvents();
}
}