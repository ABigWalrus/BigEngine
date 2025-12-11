#include "core.hpp"

int main() {
    App app{};

    auto entity1 = app.create_entity("test1");
    auto entity2 = app.create_entity("test2");

    entity1.add_child(&entity2);

    entity1.translate(glm::vec3{1.0, 1.0, 0.0});
    entity1.update();

    entity1.log_info();
    entity2.log_info();
}
