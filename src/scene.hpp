#pragma once

#include "entity.hpp"
#include "util.hpp"
#include <unordered_map>

class Scene {
    std::unordered_map<Id, Entity, IdHash> m_entity_map;

    Entity *create_entity();
};
