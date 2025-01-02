#include "game_object.hpp"
#include <iostream>

game_object::game_object(const shader *object_shader, const std::string &path,
                         double mass, glm::vec3 velocity, double xpos,
                         double ypos, double zpos)
    : object(object_shader, path, xpos, ypos, zpos), entity(mass, velocity) {}

game_object::~game_object() {}

void game_object::evaluate(double delta_time) {
    entity::evaluate(delta_time);
    glm::vec3 velocity = entity::get_velocity();
    this->translate(velocity * (float)delta_time);
}

glm::vec3 calculate_gravity(const game_object *obj1, const game_object *obj2) {
    glm::vec3 direction = obj1->get_position() - obj2->get_position();
    float distance = glm::length(direction);
    return glm::normalize(direction) *
           obj1->get_gravity(distance, obj2->get_mass());
}

void resolve_gravity(std::vector<game_object *> objects) {
    size_t i = 0, j = 1;
    for (; j < objects.size(); i++, j++) {
        glm::vec3 force = calculate_gravity(objects[i], objects[j]);
        objects[j]->apply_force(force);
        objects[i]->apply_force(-force);
    }
    if (objects.size() % 2 == 1) {
        glm::vec3 force = calculate_gravity(objects[i], objects[0]);
        objects[0]->apply_force(force);
        objects[i]->apply_force(-force);
    }
}
