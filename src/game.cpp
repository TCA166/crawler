#include "game.hpp"
#include <iostream>

game::game()
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), time_scale(2e-5) {
    this->current_time = glfwGetTime();
    this->delta_time = 0.0;
    this->xpos = 0.0;
    this->ypos = 0.0;
    this->mv_forward = false;
    this->mv_backward = false;
    this->mv_left = false;
    this->mv_right = false;
    this->mv_up = false;
    this->mv_down = false;
}

game::~game() {
    delete this->textured_shader;
    delete this->skybox_shader;
    delete this->cube1;
    delete this->cube2;
    delete this->wall;
    delete this->floor;
    delete this->lght;
    delete this->sky;
}

void game::init() {
    textured_shader = new shader(SHADER_PATH("textured_vertex.glsl"),
                                 SHADER_PATH("textured_fragment.glsl"));
    cube1 = new debug_cube(textured_shader, 0.0, 0.1, 0.0);
    this->add_object(cube1);
    cube2 = new debug_cube(textured_shader, 1.0, 0.1, 1.0);
    this->add_object(cube2);
    wall = new debug_wall(textured_shader, 0.0, 1.0, -5.0);
    wall->set_scale(10.0, 3.0, 1.0);
    this->add_object(wall);
    floor = new debug_wall(textured_shader, 0.0, -0.5, 0.0);
    floor->set_rotation(1.57, 0.0, 0.0);
    floor->set_scale(10.0, 10.0, 1.0);
    this->add_object(floor);
    lght = new light(glm::vec3(4.0, 1.0, 4.0), glm::vec3(0.0, 0.0, -1.0),
                     glm::vec3(1.0, 1.0, 1.0));
    skybox_shader = new shader(SHADER_PATH("skybox_vertex.glsl"),
                               SHADER_PATH("skybox_fragment.glsl"));
    std::vector<std::string> paths = {
        TEXTURE_PATH("skybox/right.png"), TEXTURE_PATH("skybox/left.png"),
        TEXTURE_PATH("skybox/top.png"),   TEXTURE_PATH("skybox/bottom.png"),
        TEXTURE_PATH("skybox/front.png"), TEXTURE_PATH("skybox/back.png")};
    sky = new skybox(skybox_shader, paths);
    this->set_skybox(sky);
    this->add_light(lght);
    scene::init();
}

void game::main(camera *target_camera) {
    target_camera->set_speed(10.0f);
    while (!this->get_should_close()) {
        {
            double new_time = glfwGetTime();
            delta_time = new_time - current_time;
            current_time = new_time;
        }
        glfwPollEvents();
        if (mv_forward) {
            target_camera->move_forward(delta_time);
        } else if (mv_backward) {
            target_camera->move_forward(-delta_time);
        }
        if (mv_left) {
            target_camera->move_right(-delta_time);
        } else if (mv_right) {
            target_camera->move_right(delta_time);
        }
        if (mv_up) {
            target_camera->move_up(delta_time);
        } else if (mv_down) {
            target_camera->move_up(-delta_time);
        }
        delta_time *= time_scale;
        glm::vec3 lght_pos = lght->get_position();
        lght_pos.x = sin(current_time) * 4.0;
        lght->set_position(lght_pos);
    }
}

void game::scroll_callback(double, double yoffset, camera *target_camera) {
    target_camera->zoom(yoffset);
}

void game::key_callback(int key, int, int action, int, camera *) {
    if (action == GLFW_REPEAT) {
        return;
    }
    bool pressed = action == GLFW_PRESS;
    switch (key) {
    case GLFW_KEY_W:
        mv_forward = pressed;
        break;
    case GLFW_KEY_S:
        mv_backward = pressed;
        break;
    case GLFW_KEY_A:
        mv_left = pressed;
        break;
    case GLFW_KEY_D:
        mv_right = pressed;
        break;
    case GLFW_KEY_SPACE:
        mv_up = pressed;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        mv_down = pressed;
        break;
    }
}

void game::mouse_callback(double xpos, double ypos, camera *target_camera) {
    if (xpos != this->xpos || ypos != this->ypos) {
        target_camera->rotate_front(xpos - this->xpos, ypos - this->ypos);
        this->xpos = xpos;
        this->ypos = ypos;
    }
}
