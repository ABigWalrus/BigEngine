#include "scene.hpp"
#include "utility.hpp"
// #include "Engine/entity.hpp"


Scene::Scene():m_uuid(util::generate_uuid()){}
Scene::~Scene(){}

void Scene::add(Entity&& entity) {
    m_entities.push_back(entity);
    // entity = nullptr;
}


std::ostream& operator<<(std::ostream& os, const Scene& scene) {
    os << "Scene " << scene.m_uuid;
    for(Entity entity: scene.m_entities) {
        os << '\n' << entity;
    }

    return os;
}