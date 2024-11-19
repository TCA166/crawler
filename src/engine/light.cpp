#include "light.hpp"

light::light(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) : position(position), color(color), constant(constant), linear(linear), quadratic(quadratic) {
    
}

light::~light() {
    
}

const glm::vec3* light::get_position() const {
    return &position;
}

const glm::vec3* light::get_color() const {
    return &color;
}

float light::get_constant() const {
    return constant;
}

float light::get_linear() const {
    return linear;
}

float light::get_quadratic() const {
    return quadratic;
}
