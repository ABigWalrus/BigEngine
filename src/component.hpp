#pragma once

#include "./util.hpp"

#include <iostream>
#include <vector>

class Component {
  public:
  private:
    Id m_id;

    Component() : m_id(generate_id()) {}

    friend std::ostream &operator<<(std::ostream &os, const Component &comp);
    friend std::ostream &operator<<(std::ostream &os,
                                    const std::vector<Component> &components);
    friend Component spawn_component();
};

inline std::ostream &operator<<(std::ostream &os, const Component &comp) {
    os << "Component with " << comp.m_id;
    return os;
}

inline std::ostream &operator<<(std::ostream &os,
                                const std::vector<Component> &components) {
    os << components.size() << " Components:";
    for (const Component &component : components) {
        os << "\n - " << component;
    }
    return os;
}

inline Component spawn_component() {
    std::cout << "Spawning Component\n";
    return Component{};
}
