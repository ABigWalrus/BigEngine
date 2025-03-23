#pragma once

#include "component.hpp"
#include <stdint.h>
#include <unordered_map>

class System {
  public:
    System();
    virtual ~System();

    virtual Component *create_component();

  private:
    using UUID = uint32_t;
    std::unordered_map<UUID, Component> m_component_map
};