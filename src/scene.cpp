#include "scene.hpp"
#include "entity.hpp"
#include "util.hpp"
#include <unordered_map>
// #include "entity.hpp"

Entity *Scene::create_entity() {
    Entity new_entity = Entity{};
    Id id = new_entity.get_id();

    m_entity_map.emplace(id, std::move(new_entity));

    return &m_entity_map.at(id);
}
