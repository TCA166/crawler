#include "entity.hpp"

entity::entity(double mass, glm::vec3 momentum) : mass(mass), momentum(momentum) {

}

double entity::get_gravity(double distance, double mass) const {
    return (this->mass * mass) / (distance * distance)
}

void entity::apply_force(glm::vec3 force) {
    this->momentum += force;
}

double entity::get_mass() const {
    return mass;
}

glm::vec3 entity::get_momentum() const {
    return momentum;
}
