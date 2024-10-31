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
    #include "Box.cpp"
    object box_obj = object(&box_shader, box, sizeof(box));
    object box_2 = object(&box_shader, box, sizeof(box), 1.0, 1.0, 1.0);
    texture ship_texture = texture("textures/spaceship.jpg", "texture0");
    texture ship_normal = texture("textures/spaceship_normal.jpg", "normal0");
    object ship = object(&textured_shader, "models/spaceship.obj", -1.0, 0.0, 0.0);
    ship.add_texture(&ship_texture);
    ship.add_texture(&ship_normal);

    current_scene.add_object(box_obj);
    current_scene.add_object(box_2);
    current_scene.add_object(ship);

    // renderer loop
    current_renderer.run();

    glfwTerminate();
    return 0;
}


