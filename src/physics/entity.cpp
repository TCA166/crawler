#include "entity.hpp"

#include "constants.hpp"

entity::entity(float mass, glm::vec3 velocity)
    : mass(mass), velocity(velocity), force_accumulator(glm::vec3(0.0f)) {}

float entity::get_gravity(float distance, float mass) const {
  return calculate_gravity(this->mass, mass, distance);
}

void entity::apply_force(glm::vec3 force) { force_accumulator += force; }

float entity::get_mass() const { return mass; }

glm::vec3 entity::get_velocity() const { return velocity; }

void entity::evaluate(double delta_time) {
  glm::vec3 acceleration = force_accumulator / mass;
  velocity += acceleration * (float)delta_time;
  force_accumulator = glm::vec3(0.0f);
}

entity::~entity() {}
