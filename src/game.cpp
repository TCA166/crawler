#include "game.hpp"

#define SCALE 10.0

game::game() : scene(glm::vec3(0.0), glm::vec3(0.0)) {
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
    delete this->our;
    delete this->textured_shader;
    delete this->sun;
    delete this->earth_obj;
    delete this->planet_shader;
}

void game::init() {
    textured_shader = new shader("shaders/textured_vertex.glsl", "shaders/textured_fragment.glsl");
    our = new ship(textured_shader, 1.0, 0.0, 0.0);
    
    moon_obj = new moon(textured_shader, SCALE, 0.0, 0.0);
    
    planet_shader = new shader("shaders/textured_vertex.glsl", "shaders/planet_fragment.glsl");
    earth_obj = new earth(planet_shader, 0.0, 0.0, 0.0);
    
    sun = new (light){glm::vec3(-5.0f, 2.5f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f};

    this->add_light(sun);
    this->add_object(earth_obj);
    this->add_object(our);
    this->add_object(moon_obj);
    scene::init();
}

void game::main(camera* target_camera) {
    while(!this->get_should_close()){
        glfwPollEvents();
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
        earth_obj->set_rotation(0.0, -current_time * 0.05, 0.0);
        moon_obj->set_rotation(0.0, -current_time * 0.1, 0.0);
        {
            glm::vec3 direction = earth_obj->get_position() - moon_obj->get_position();
            float distance = glm::length(direction);
            glm::vec3 gravity = glm::normalize(direction) * earth_obj->get_gravity(distance, moon_obj->get_mass());
            moon_obj->apply_force(gravity);
        }
        moon_obj->evaluate(delta_time * 1e-5);
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
