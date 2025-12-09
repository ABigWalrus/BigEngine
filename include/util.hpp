#pragma once

// #include "glm/fwd.hpp"
#include <cstddef>
#include <cstdint>
// #include <functional>
#include <glm/glm.hpp>
#include <iostream>

struct Id {
  public:
    inline bool operator==(const Id &other_id) const {
        return this->m_value == other_id.m_value;
    }

    inline bool operator!=(const Id &other_id) const {
        return this->m_value != other_id.m_value;
    }

    // Delete copy and move operations
    // ID(const ID &) = delete;
    // ID &operator=(const ID &) = delete;
    // ID(ID &&) = delete;
    // ID &operator=(ID &&) = delete;

  private:
    inline static uint32_t m_GLOBAL_COUNTER = 0;
    const uint32_t m_value;

    Id(uint32_t value) : m_value{value} {}

    friend std::ostream &operator<<(std::ostream &os, const Id &id);
    friend Id generate_id();
    friend struct IdHash;
};

inline std::ostream &operator<<(std::ostream &os, const Id &id) {
    os << "Id: " << id.m_value;
    return os;
}

inline Id generate_id() {
    Id::m_GLOBAL_COUNTER++;
    return Id{Id::m_GLOBAL_COUNTER};
}

struct IdHash {
  public:
    size_t operator()(const Id &id_key) const { return id_key.m_value; }
};
