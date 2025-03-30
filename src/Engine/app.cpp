#include <chrono>
#include <iostream>
#include <string>

#include "Render/render.hpp"
#include "app.hpp"
// #include "window.hpp"

Application::Application() {
    init();
    setup();
}

Application::~Application() { cleanup(); }

void Application::run() {
    while (m_window->is_open()) {
        glfwPollEvents();
        m_system.run();
    }
}

void Application::init() {
    // renderer.init(window.getWindow(), window.getWidth(), window.getWidth());
}

void Application::cleanup() {
    // renderer.cleanup();
}