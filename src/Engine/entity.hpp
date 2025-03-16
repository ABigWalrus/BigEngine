#pragma once

#include "glm/glm.hpp"

#include <ostream>

class Entity{
public:
    Entity(int uuid);
    ~Entity();

    friend std::ostream& operator<<(std::ostream& os, const Entity& enitity);
private:
    // glm::mat4 _localMatrix;
    int _uuid;

    glm::vec3 _position;
    glm::mat4 _worldMatrix;
};