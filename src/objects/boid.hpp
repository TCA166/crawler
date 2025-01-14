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
   @param deltaTime The time since the last update
  */
  void update(const std::vector<const boid *> &boids, double deltaTime);
  /*!
   @brief Checks if the boid is active
   @return True if the boid is active
  */
  bool is_active() const;
  /*!
   @brief Activates the boid
  */
  void activate();
  /*!
   @brief Deactivates the boid
  */
  void deactivate();

private:
  void evaluate(double deltaTime);

  bool active;

  glm::vec3 separation(const std::vector<const boid *> &boids);
  glm::vec3 alignment(const std::vector<const boid *> &boids);
  glm::vec3 cohesion(const std::vector<const boid *> &boids);

  static constexpr float maxSpeed = 8.0f;
  static constexpr float maxForce = 1.0f;
  static constexpr float separationDistance = 2.0f;
  static constexpr float alignmentDistance = 8.0f;
  static constexpr float cohesionDistance = 4.0f;

  const texture *tex;
};

inline boid::boid(const shader *object_shader, double xpos, double ypos,
                  double zpos, const texture *tex)
    : triangle(object_shader, xpos, ypos, zpos),
      entity(1.0f, glm::sphericalRand(1.0f)), active(true), tex(tex) {
  this->add_texture(tex, "texture");
}

inline boid::~boid() {}

inline bool boid::is_active() const { return active; }

inline void boid::activate() { active = true; }

inline void boid::deactivate() { active = false; }

inline void boid::update(const std::vector<const boid *> &boids,
                         double deltaTime) {
  deltaTime *= 1.0;

  glm::vec3 sep = separation(boids) * 4.0f;
  glm::vec3 ali = alignment(boids) * 1.0f;
  glm::vec3 coh = cohesion(boids) * 0.5f;

  this->apply_force(sep + ali + coh);

  this->evaluate(deltaTime);

  this->translate(velocity * (float)deltaTime);

  // Boundary conditions
  if (xpos < -7.0f || xpos > 7.0f) {
    velocity.x = -velocity.x;
  }
  if (ypos < -7.0f || ypos > 7.0f) {
    velocity.y = -velocity.y;
  }
  if (ypos < -7.0f || ypos > 7.0f) {
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

inline glm::vec3 boid::separation(const std::vector<const boid *> &boids) {
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

inline glm::vec3 boid::alignment(const std::vector<const boid *> &boids) {
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

inline glm::vec3 boid::cohesion(const std::vector<const boid *> &boids) {
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
