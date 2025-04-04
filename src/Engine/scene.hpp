#pragma once

#include "entity.hpp"
#include <ostream>
#include <unordered_map>

class Scene {
  public:
    Scene();
    ~Scene();

    void add(Entity &&enitity);

    friend std::ostream &operator<<(std::ostream &os, const Scene &scene);

  private:
    int m_uuid;
    std::unordered_map<int, Entity> m_entities;
};