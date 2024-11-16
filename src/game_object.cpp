#include "game_object.hpp"

#include <iostream>

game_object::game_object(const shader* object_shader, const std::string& path, double mass, glm::vec3 velocity, double xpos, double ypos, double zpos) : object(object_shader, path, xpos, ypos, zpos), entity(mass, velocity) {
    
}

game_object::~game_object() {
    
}

void game_object::evaluate(double delta_time) {
    entity::evaluate(delta_time);
    glm::vec3 velocity = entity::get_velocity();
    this->translate(velocity * (float)delta_time);
}
