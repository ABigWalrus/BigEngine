#include "scene.hpp"
// #include "Engine/entity.hpp"


Scene::Scene(int uuid):_uuid(uuid){}
Scene::~Scene(){}

void Scene::add(Entity&& entity) {
    _entities.push_back(entity);
    // entity = nullptr;
}

// int Scene::get_uuid() { return _uuid; }

std::ostream& operator<<(std::ostream& os, const Scene& scene) {
    os << "Scene " << scene._uuid;
    for(Entity entity: scene._entities) {
        os << entity;
    }

    return os;
}