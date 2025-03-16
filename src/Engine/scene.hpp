#pragma once

#include <ostream>
#include <vector>
#include "entity.hpp"



class Scene{
public:
    
    Scene(int uuid);
    ~Scene();
    
    void add(Entity&& enitity);

    // int get_uuid();

    friend std::ostream& operator<<(std::ostream& os, const Scene& scene);
private:
    int _uuid;
    std::vector<Entity> _entities;
};