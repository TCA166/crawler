#include "game.hpp"
#include <iostream>

game::game() : scene(glm::vec3(0.0), glm::vec3(0.0)), time_scale(1e-6), earth_obj(nullptr), moon_obj(nullptr) {
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
    delete this->sun_obj;
    delete this->earth_obj;
    delete this->planet_shader;
}

void game::init() {
    textured_shader = new shader("shaders/textured_vertex.glsl", "shaders/textured_fragment.glsl");
    our = new ship(textured_shader, 1.0, 0.0, 0.0);
    
    planet_shader = new shader("shaders/textured_vertex.glsl", "shaders/planet_fragment.glsl");
    earth_obj = new earth(planet_shader, 0.0, 0.0, 0.0);
    
    moon_obj = new moon(textured_shader, -earth_moon_distance, 0.0, 0.0);

    sun_shader = new shader("shaders/textured_vertex.glsl", "shaders/sun_fragment.glsl");
    sun_obj = new sun(sun_shader, earth_sun_distance, 0.0, 0.0);

    this->add_light(sun_obj);
    this->add_object(sun_obj);
    this->add_object(earth_obj);
    this->add_object(our);
    this->add_object(moon_obj);
    scene::init();
}

void game::main(camera* target_camera) {
    std::vector<game_object*> objects = {earth_obj, moon_obj, sun_obj};
    while(!this->get_should_close()){
        {
            double new_time = glfwGetTime();
            delta_time = new_time - current_time;
            current_time = new_time;
        }
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
        delta_time *= time_scale;
        earth_obj->set_rotation(0.0, -current_time * 0.05, 0.0);
        moon_obj->set_rotation(0.0, -current_time * 0.1, 0.0);
        resolve_gravity(objects); 
        moon_obj->evaluate(delta_time);
        sun_obj->evaluate(delta_time);
        glm::vec3 earth_pos = earth_obj->get_position();
        earth_obj->evaluate(delta_time);
        target_camera->set_position(earth_obj->get_position() + target_camera->get_position() - earth_pos);
        std::cout << "earth-moon distance: " << glm::length(earth_obj->get_position() - moon_obj->get_position()) << std::endl;
        /*
        std::cout << "moon" << moon_obj->get_position().x << " " << moon_obj->get_position().y << " " << moon_obj->get_position().z << std::endl;
        std::cout << "earth" << earth_obj->get_position().x << " " << earth_obj->get_position().y << " " << earth_obj->get_position().z << std::endl;
        std::cout << "sun" << sun_obj->get_position().x << " " << sun_obj->get_position().y << " " << sun_obj->get_position().z << std::endl;
        */
    }
}

void game::scroll_callback(double, double yoffset, camera* target_camera) {
    target_camera->zoom(yoffset);
}

void game::key_callback(int key, int, int action, int, camera*) {
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

void game::mouse_callback(double xpos, double ypos, camera* target_camera) {
    if(xpos != this->xpos || ypos != this->ypos){
        target_camera->rotate_front(xpos - this->xpos, ypos - this->ypos);
        this->xpos = xpos;
        this->ypos = ypos;
    }
}
