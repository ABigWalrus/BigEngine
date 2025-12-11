#include "core.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Entity::add_child(Entity *child) {
    if (child != nullptr && child->m_parent == nullptr) {
        std::cout << "Adding a child " << child->m_name << " to " << m_name
                  << std::endl;
        m_children.push_back(child);
        child->m_parent = this;
    }
}

void Entity::update() {

    std::cout << "Updating entity " << m_name << std::endl;
    if (m_dirty) {
        std::cout << "Updating matrix " << std::endl;
        glm::mat4 new_local_matrix{1.0};
        new_local_matrix = glm::scale(new_local_matrix, m_scale);
        // TODO: fix m_rotation by using glm::rotate instead of cast
        new_local_matrix = glm::mat4_cast(m_rotation) * new_local_matrix;
        m_local_matrix = glm::translate(new_local_matrix, m_position);
        m_dirty = false;
    }

    // TODO: update world matrix only if parent world matrix updated
    // or local matrix has updated
    if (m_parent == nullptr) {
        m_world_matrix = m_local_matrix;
    } else {
        m_world_matrix = m_parent->m_world_matrix * m_local_matrix;
    }

    for (const auto child : m_children) {
        child->update();
    }
}

void Entity::translate(glm::vec3 trans_vec) {
    m_position += trans_vec;
    m_dirty = true;
}

// TODO: make a log system that will get a template on how to print a message
// And entity has just to define this template
void Entity::log_info() {
    std::cout << "============\n"
              << "Entity: " << m_name << " ,id: " << m_id << "\n"
              << "Position " << m_position.x << "; " << m_position.y << "; "
              << m_position.z << ";\n"
              << "Rotation " << m_rotation.w << "; " << m_rotation.x << "; "
              << m_rotation.y << "; " << m_rotation.y << ";\n"
              << "Scale " << m_scale.x << "; " << m_scale.y << "; " << m_scale.z
              << ";\n"
              << "Local matrix:\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << m_local_matrix[i].x << "; " << m_local_matrix[i].y << "; "
                  << m_local_matrix[i].z << "; " << m_local_matrix[i].w
                  << ";\n";
    }

    std::cout << "World matrix: \n";
    for (int i = 0; i < 4; ++i) {
        std::cout << m_world_matrix[i][0] << "; " << m_world_matrix[i][1]
                  << "; " << m_world_matrix[i][2] << "; "
                  << m_world_matrix[i][3] << ";\n";
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