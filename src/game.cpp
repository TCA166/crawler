#include "game.hpp"

game::game(glm::vec3 ambient_light, glm::vec3 background_color) : scene(ambient_light, background_color) {
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
    
}

void game::init() {
    // FIXME memory leak + no way of getting those objects later
    shader* textured_shader = new shader("shaders/textured_vertex.glsl", "shaders/textured_fragment.glsl");
    texture* ship_texture = new texture("textures/spaceship.jpg");
    texture* ship_normal = new texture("textures/spaceship_normal.jpg");
    object* ship = new object(textured_shader, "models/spaceship.obj", 1.0, 1.0, 0.0);
    ship->add_texture(ship_texture, "texture0");
    ship->add_texture(ship_normal, "normal0");

    light* blue_sun = new (light){glm::vec3(5.0f, 2.5f, 5.0f), glm::vec3(1.0f, 0.2f, 0.5f), 1.0f, 0.09f, 0.032f};
    this->add_light(blue_sun);
    light* yellow_sun = new (light){glm::vec3(-5.0f, 2.5f, 5.0f), glm::vec3(0.0f, 0.9f, 0.0f), 1.0f, 0.09f, 0.032f};
    this->add_light(yellow_sun);
    this->add_object(ship);
    scene::init();
}

void game::main(camera* target_camera) {
    while(!this->get_should_close()){
        if(mv_forward){
            target_camera->move_forward(delta_time);
        } else if(mv_backward){
            target_camera->move_forward(-delta_time);
        }
        if(mv_left){
            target_camera->move_right(-delta_time);
        } else if(mv_right){
            target_camera->move_right(delta_time);
        }
        if(mv_up){
            target_camera->move_up(delta_time);
        } else if(mv_down){
            target_camera->move_up(-delta_time);
        }
        {
            double new_time = glfwGetTime();
            delta_time = new_time - current_time;
            current_time = new_time;
        }
    }
}

void game::scroll_callback(double xoffset, double yoffset, camera* target_camera) {
    target_camera->zoom(yoffset);
}

void game::key_callback(int key, int scancode, int action, int mods, camera* target_camera) {
    if(action == GLFW_REPEAT){
        return;
    }
    bool pressed = action == GLFW_PRESS;
    switch(key){
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

void game::mouse_button_callback(int button, int action, int mods, camera* target_camera) {
    // do nothing
}

void game::mouse_callback(double xpos, double ypos, camera* target_camera) {
    if(xpos != this->xpos || ypos != this->ypos){
        target_camera->rotate_front(xpos - this->xpos, ypos - this->ypos);
        this->xpos = xpos;
        this->ypos = ypos;
    }
}
