#include "light.hpp"

light::light(glm::vec3 direction, glm::vec3 color)
    : direction(direction), color(color) {}

light::~light() {}

const glm::vec3 &light::get_direction() const { return direction; }

const glm::vec3 &light::get_color() const { return color; }

void light::set_direction(glm::vec3 direction) { this->direction = direction; }

void light::set_color(glm::vec3 color) { this->color = color; }
