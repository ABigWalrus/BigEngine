#include "core.hpp"

int main() {
    App app{};
    auto entity1 = app.create_entity("test1");
    auto entity2 = app.create_entity("test2");

    entity1.add_child(&entity2);
}
