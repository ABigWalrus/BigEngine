#include "entity.hpp"

// #include <ostream>
// #include <string>

Entity::Entity(int uuid):_uuid(uuid) {}
Entity::~Entity() {}

std::ostream& operator<<(std::ostream& os, const Entity& enitity) {
    return os << "Entity " << enitity._uuid;
}
