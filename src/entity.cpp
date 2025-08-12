#include "./entity.hpp"
#include "glm/ext/matrix_transform.hpp"

void Entity::translate(const glm::vec3 &transl_vec) {
  m_position += transl_vec;
  m_local_matrix = glm::translate(m_local_matrix, transl_vec);
  // m_global_matrix = glm::translate(m_global_matrix, transl_vec);
}

// void Entity::rotate(glm::vec3 rotate_vec) { m_rotation += rotate_vec; }

void Entity::scale(glm::vec3 scale_vec) {
  m_scale += scale_vec;
  m_local_matrix = glm::scale(m_local_matrix, scale_vec);
  // m_global_matrix = glm::scale(m_global_matrix, scale_vec);
}
