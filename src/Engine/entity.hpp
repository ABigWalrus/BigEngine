#pragma once

#include "glm/glm.hpp"

#include <ostream>

class Entity {
  public:
    Entity();
    ~Entity();

    inline int get_uuid() { return m_uuid; }

    friend std::ostream &operator<<(std::ostream &os, const Entity &enitity);

  private:
    // glm::mat4 _localMatrix;
    int m_uuid;

    glm::vec3 m_position;
    glm::mat4 m_worldMatrix;
};