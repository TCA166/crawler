#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "renderer.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

int main() {
    if(glfwInit() == GLFW_FALSE){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // a scene is a collection of objects
    scene current_scene = scene();

    // renderer handles all the initialization
    renderer current_renderer = renderer(&current_scene, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

    shader box_shader = shader("shaders/box_vertex.glsl", "shaders/box_fragment.glsl");
    shader textured_shader = shader("shaders/textured_vertex.glsl", "shaders/textured_fragment.glsl");
    texture ship_texture = texture("textures/spaceship.jpg", "texture0");
    texture ship_normal = texture("textures/spaceship_normal.jpg", "normal0");
    textured_object ship = textured_object(&textured_shader, "models/spaceship.obj", -1.0, 0.0, 0.0);
    ship.add_texture(&ship_texture);
    ship.add_texture(&ship_normal);

    light blue_sun = (light){glm::vec3(5.0f, 2.5f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.09f, 0.032f};
    current_scene.add_light(&blue_sun);
    light yellow_sun = (light){glm::vec3(-1.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f, 0.09f, 0.032f};
    current_scene.add_light(&yellow_sun);
    current_scene.add_object((object*)&ship);

    // renderer loop
    current_renderer.run();

    glfwTerminate();
    return 0;
}


