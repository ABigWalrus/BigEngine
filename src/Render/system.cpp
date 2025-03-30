#include "system.hpp"
#include "Render/device.hpp"
#include "Render/info.hpp"
#include "Render/render.hpp"
#include <memory>

RenderSystem::RenderSystem(std::shared_ptr<Big::Window> window)
    : m_window{window} {
    m_device = std::make_shared<Big::Device>(m_window);
    m_info =
        std::make_shared<RenderInformation>("../assets/models/viking_room.obj");
    m_renderer = std::make_shared<Big::Renderer>(m_window, m_device, m_info);
}

void RenderSystem::run() { m_renderer->render(); }

RenderSystem::~RenderSystem() {}