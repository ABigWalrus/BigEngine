#pragma once

// #include "component.hpp"
// #include "scene.hpp"
// #include "glm/fwd.hpp"
#include "glm/fwd.hpp"
#include "util.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>

class Entity {
  public:
    void add_child(Entity *child);
    void update();

    void translate(glm::vec3 trans_vec);

    void log_info();

    Entity(std::string name);

  private:
    const Id m_id{generate_id()};
    std::string m_name;

    std::vector<Entity *> m_children{};
    Entity *m_parent = nullptr;

    glm::vec3 m_position{0.0, 0.0, 0.0};
    glm::quat m_rotation{0.0, 0.0, 0.0, 0.0};
    glm::vec3 m_scale{0.0, 0.0, 0.0};

    glm::mat4 m_local_matrix{1.0};
    glm::mat4 m_world_matrix{1.0};

    bool m_dirty = false;
};

class App {
  public:
    Entity &create_entity(std::string name);

  private:
    std::vector<Entity> m_entities{};
};
//   public:
//     // inline void add_component(Component &cmp) {
//     m_components.push_back(cmp);
//     // }
//     inline void add_child(Entity &ent) { m_children.push_back(ent); }

//     void translate(const glm::vec3 &transl_vec);
//     // void rotate(glm::vec3 rotate_vec);
//     void scale(glm::vec3 scale_vec);

//     Id get_id() const { return m_id; }

//   private:
//     Id m_id;

//     // std::vector<Component> m_components;
//     std::vector<Entity> m_children;

//     glm::mat4 m_local_matrix;
//     glm::mat4 m_global_matrix;

//     glm::vec3 m_position; // local position
//     glm::vec3 m_rotation; // local rotation
//     glm::vec3 m_scale;    // local scale

//     Entity()
//         : m_id(generate_id()), m_children{}, m_position(0.0, 0.0, 0.0),
//           m_rotation(0.0, 0.0, 0.0), m_scale(0.0, 0.0, 0.0),
//           m_local_matrix(1), m_global_matrix(1) {}

//     // void update();
//     // void update(glm::mat4 parent_matrix);

//     friend std::ostream &operator<<(std::ostream &os, const Entity &ent);
//     friend std::ostream &operator<<(std::ostream &os,
//                                     const std::vector<Entity> &entities);
//     // friend Entity spawn_entity();
//     friend struct Scene;
// };

// inline std::ostream &operator<<(std::ostream &os, const Entity &ent) {
//     os << "Entity with " << ent.m_id << ":\n";
//     os << " - Position: {x:" << ent.m_position.x << ", y:" <<
//     ent.m_position.y
//        << ", z: " << ent.m_position.x << "}\n";
//     os << " - Rotation: {x:" << ent.m_rotation.x << ", y:" <<
//     ent.m_rotation.y
//        << ", z: " << ent.m_rotation.x << "}\n";
//     os << " - Scale: {x:" << ent.m_scale.x << ", y:" << ent.m_scale.y
//        << ", z: " << ent.m_scale.x << "}\n";

//     os << " - Local matrix: [ ";
//     for (int y = 0; y < 4; y++) {
//         for (int x = 0; x < 4; x++) {
//             os << ent.m_local_matrix[y][x] << ", ";
//         }
//     }
//     os << " ]\n";

//     os << " - Global matrix: [ ";
//     for (int y = 0; y < 4; y++) {
//         for (int x = 0; x < 4; x++) {
//             os << ent.m_global_matrix[y][x] << ", ";
//         }
//     }
//     os << " ]\n";

//     if (ent.m_components.size() == 0) {
//         os << "No components" << "\n";
//     } else {
//         os << ent.m_components << "\n";
//     }

//     if (ent.m_children.size() == 0) {
//         os << "No Children";
//     } else {
//         os << ent.m_children;
//     }
//     return os;
// }

// inline std::ostream &operator<<(std::ostream &os,
//                                 const std::vector<Entity> &enttities) {
//     os << enttities.size() << " Entities:";

//     for (const Entity &entity : enttities) {
//         os << "\n - " << entity;
//     }
//     return os;
// }

// // inline Entity spawn_entity() {
// //   std::cout << "Spawning Enityt\n";
// //   return Entity{};
// // }
