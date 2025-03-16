#include "scene.hpp"
#include "Engine/entity.hpp"
#include "utility.hpp"
// #include "Engine/entity.hpp"

Scene::Scene() : m_uuid(util::generate_uuid()) {}
Scene::~Scene() {}

void Scene::add(Entity &&entity) {
    m_entities.insert({entity.get_uuid(), entity});
    // entity = nullptr;
}

std::ostream &operator<<(std::ostream &os, const Scene &scene) {
    os << "Scene " << scene.m_uuid;
    for (const std::pair<int, Entity> &entity : scene.m_entities) {
        os << '\n' << entity.second;
    }

    return os;
}