#pragma once

#include "Engine/utility.hpp"
#include "Render/device.hpp"
#include "Render/window.hpp"
#include "info.hpp"
#include "render.hpp"

#include <memory>
#include <unordered_map>

class RenderSystem {
  public:
    RenderSystem(std::shared_ptr<Big::Window> const &window);
    ~RenderSystem();

    UUID create_info(std::string model_path);
    RenderInformation *get_info(UUID uuid);

    void update();

  private:
    std::shared_ptr<Big::Window> m_window;
    std::shared_ptr<Big::Device> m_device;
    std::unique_ptr<Big::Renderer> m_renderer;

    std::unordered_map<UUID, RenderInformation> m_info_map;
};