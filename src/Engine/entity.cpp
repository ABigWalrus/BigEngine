#include "entity.hpp"
#include "utility.hpp"
// #include <ostream>
// #include <string>

Entity::Entity() : m_uuid(util::generate_uuid()) {}
Entity::~Entity() {}

std::ostream &operator<<(std::ostream &os, const Entity &enitity) {
    return os << "Entity " << enitity.m_uuid;
}
