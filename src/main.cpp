#include "Engine/app.hpp"
#include "Engine/entity.hpp"
#include "Engine/scene.hpp"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>

void Application::setup() {
    Scene scene;

    Entity enitity3;
    scene.add(Entity{});
    scene.add(std::move(enitity3));

    std::cout << scene << std::endl;
}

int main() {

    Application app1;
    try {
        // app1.init();
        app1.run();
        // app1.cleanup();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// void setup() {
//     Scene scene = new Scene(); // I may call a world later
//     Enitity boris = new Entitity();
//     Camera mainCamera = new Camera();

//     boris.bind(new RenderInformation(file));
//     boris.bind(new Texture(file));

//     scene.add(std::move(boris));
//     scene.add(std::move(mainCamera));

//     this.render(std::move(Scene));
// }

// app.setup();
// app,run();