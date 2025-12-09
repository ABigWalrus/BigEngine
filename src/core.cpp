#include "core.hpp"

void Entity::add_child(Entity *child) {
    if (child != nullptr && child->m_parent == nullptr) {
        std::cout << "Adding a child " << child->m_name << " to " << m_name
                  << std::endl;
        m_children.push_back(child);
        child->m_parent = this;
    }
}

Entity::Entity(std::string name) : m_name{name} {
    std::cout << "Creating entity " << m_name << std::endl;
}

Entity &App::create_entity(std::string name) {
    Entity entity{name};
    m_entities.push_back(entity);
    return m_entities.back();
}

// inline std::ostream &operator<<(std::ostream &os,
//                                 const std::vector<Component> &components) {
//     os << components.size() << " Components:";
//     for (const Component &component : components) {
//         os << "\n - " << component;
//     }
//     return os;
// }