#include "include.hpp"

#include <iostream>
#include <thread>

#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "renderer.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

static void render_thread(renderer* instance){
    renderer second_renderer = instance->clone("Second Window");
    second_renderer.run();
}

int main() {
    if(glfwInit() == GLFW_FALSE){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    float ambient_strength = 0.3f;
    // a scene is a collection of objects
    scene current_scene = scene(glm::vec3(ambient_strength, ambient_strength, ambient_strength), glm::vec3(0.0f, 0.3f, 0.3f));

    sem_t semaphore;
    sem_init(&semaphore, 0, 1);

    // renderer handles all the initialization
    renderer current_renderer = renderer(&current_scene, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, &semaphore);

    shader textured_shader = shader("shaders/textured_vertex.glsl", "shaders/textured_fragment.glsl");
    texture ship_texture = texture("textures/spaceship.jpg");
    texture ship_normal = texture("textures/spaceship_normal.jpg");
    object ship = object(&textured_shader, "models/spaceship.obj", 1.0, 1.0, 0.0);
    ship.add_texture(&ship_texture, "texture0");
    ship.add_texture(&ship_normal, "normal0");

    light blue_sun = (light){glm::vec3(5.0f, 2.5f, 5.0f), glm::vec3(1.0f, 0.2f, 0.5f), 1.0f, 0.09f, 0.032f};
    current_scene.add_light(&blue_sun);
    light yellow_sun = (light){glm::vec3(-5.0f, 2.5f, 5.0f), glm::vec3(0.0f, 0.9f, 0.0f), 1.0f, 0.09f, 0.032f};
    current_scene.add_light(&yellow_sun);
    current_scene.add_object((object*)&ship);

    //std::thread renderer_thread = std::thread(render_thread, &current_renderer);
    // renderer loop
    current_renderer.run();
    //renderer_thread.join();

    glfwTerminate();
    return 0;
}


