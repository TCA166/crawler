#pragma once

#include "../engine/engine.hpp"
#include "../physics/entity.hpp"

/*!
 @brief A boid object
 @details A boid object that can be used in a flocking simulation
*/
class boid : public triangle, protected entity {
public:
  /*!
   @brief Constructs a boid object
   @param object_shader The shader to use for rendering
   @param xpos The x position of the boid
   @param ypos The y position of the boid
   @param zpos The z position of the boid
   @param tex The texture of the boid
  */
  boid(const shader *object_shader, double xpos, double ypos, double zpos,
       const texture *tex);
  ~boid();

  /*!
   @brief Updates the boid, including physics and flocking
   @param boids The boids to flock with
   @param scene The collider to check collisions against
   @param deltaTime The time since the last update
  */
  void update(const std::list<const boid *> &boids, const collider *scene,
              double deltaTime);

private:
  void evaluate(double deltaTime);

  glm::vec3 separation(const std::list<const boid *> &boids);
  glm::vec3 alignment(const std::list<const boid *> &boids);
  glm::vec3 cohesion(const std::list<const boid *> &boids);

  static constexpr float maxSpeed = 8.0f;
  static constexpr float maxForce = 1.0f;
  static constexpr float separationDistance = 2.0f;
  static constexpr float alignmentDistance = 10.0f;
  static constexpr float cohesionDistance = 20.0f;

  const texture *tex;
  const texture *tex2;
  const shader *triangle_object_shader;
};

inline boid::boid(const shader *object_shader, double xpos, double ypos,
                  double zpos, const texture *tex)
    : triangle(object_shader, xpos, ypos, zpos),
      entity(1.0f, glm::sphericalRand(0.5f)), tex(tex) {
  triangle_object_shader = object_shader;
  this->add_texture(tex, "diamond");
  this->set_scale(0.5f);
}

inline boid::~boid() {}

inline void boid::update(const std::list<const boid *> &boids,
                         const collider *scene, double deltaTime) {

  glm::vec3 sep = separation(boids) * 4.0f;
  glm::vec3 ali = alignment(boids) * 1.0f;
  glm::vec3 coh = cohesion(boids) * 0.5f;

  this->apply_force(sep + ali + coh);

  this->evaluate(deltaTime);

  glm::vec3 translation = velocity * (float)deltaTime;

  glm::vec3 position = this->get_position();
  glm::vec3 target = position + translation;

  glm::vec3 bound = get_bounds();
  glm::vec3 negbound = get_negbounds();

  if (scene->check_line(position, target) ||
      scene->check_line(bound, bound + translation) ||
      scene->check_line(negbound, negbound + translation)) {
    this->velocity = -this->velocity;
  } else {
    this->set_position(target.x, target.y, target.z);
  }

  // Boundary conditions
  if (xpos < -10.0f || xpos > 10.0f) {
    velocity.x = -velocity.x;
  }
  if (ypos < -10.0f || ypos > 10.0f) {
    velocity.y = -velocity.y;
  }
  if (zpos < -10.0f || zpos > 10.0f) {
    velocity.z = -velocity.z;
  }
  return;
}

inline void boid::evaluate(double deltaTime) {
  entity::evaluate(deltaTime);
  if (glm::length(velocity) > maxSpeed) {
    velocity = glm::normalize(velocity) * maxSpeed;
  }
}

inline glm::vec3 boid::separation(const std::list<const boid *> &boids) {
  glm::vec3 steer(0.0f);
  int count = 0;
  for (const boid *other : boids) {
    glm::vec3 diff = this->get_position() - other->get_position();
    float distance = glm::length(diff);
    if (distance > 0 && distance < separationDistance) {
      diff = glm::normalize(diff) / distance;
      steer += diff;
      count++;
    }
  }
  if (count > 0) {
    steer /= static_cast<float>(count);
  }
  if (glm::length(steer) > 0) {
    steer = glm::normalize(steer) * maxSpeed - velocity;
    if (glm::length(steer) > maxForce) {
      steer = glm::normalize(steer) * maxForce;
    }
  }
  return steer;
}

inline glm::vec3 boid::alignment(const std::list<const boid *> &boids) {
  glm::vec3 sum(0.0f);
  int count = 0;
  for (const boid *other : boids) {
    float distance = glm::length(this->get_position() - other->get_position());
    if (distance > 0 && distance < alignmentDistance) {
      sum += other->get_velocity();
      count++;
    }
  }
  if (count > 0) {
    sum /= static_cast<float>(count);
    sum = glm::normalize(sum) * maxSpeed;
    glm::vec3 steer = sum - velocity;
    if (glm::length(steer) > maxForce) {
      steer = glm::normalize(steer) * maxForce;
    }
    return steer;
  }
  return glm::vec3(0.0f);
}

inline glm::vec3 boid::cohesion(const std::list<const boid *> &boids) {
  glm::vec3 sum(0.0f);
  int count = 0;
  glm::vec3 position = this->get_position();
  for (const triangle *other : boids) {
    glm::vec3 other_position = other->get_position();
    float distance = glm::length(position - other_position);
    if (distance > 0 && distance < cohesionDistance) {
      sum += other_position;
      count++;
    }
  }
  if (count > 0) {
    sum /= static_cast<float>(count);
    return glm::normalize(sum - position) * maxSpeed - velocity;
  }
  return glm::vec3(0.0f);
}
