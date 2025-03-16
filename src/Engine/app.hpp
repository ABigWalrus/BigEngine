#pragma once

#include "Render/device.hpp"
#include "Render/render.hpp"
#include "Render/window.hpp"
#include "entity.hpp"
#include "scene.hpp"

#include <string>

class Application {
  public:
    // std::string name;
    // uint64_t renders = 0;
    static constexpr uint32_t WIDTH = 1600;
    static constexpr uint32_t HEIGTH = 720;

    Application();
    ~Application();
    // std::string name, uint32_t width, uint32_t height);
    void run();
    virtual void setup();
    // void init();
    // void cleanup();

  private:
    Big::Window window{"TEST WINDOW", WIDTH, HEIGTH};
    Big::Device device{window};
    Big::Renderer renderer{window, device};

    // void initWindow();
    // void mainLoop();
    void init();
    void cleanup();
};