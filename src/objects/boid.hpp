#pragma once

#include "../engine/engine.hpp"
#include "../physics/entity.hpp"

#define SEP_SCALE 4.0f
#define ALI_SCALE 1.0f
#define COH_SCALE 0.5f
#define GROUND_SEP_SCALE 0.4f
#define PREF_Y_SCALE 0.001f

#define MAX_FORCE 1.0f

struct boid_species {
  float sep_dist; // separation distance
  float ali_dist; // alignment distance
  float coh_dist; // cohesion distance
  float max_speed;
  float pref_y;
};

const static boid_species default_boid_species = {2.0f, 10.0f, 20.0f, 8.0f,
                                                  20.0f};

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
   @param species The species of the boid
  */
  boid(const shader *object_shader, double xpos, double ypos, double zpos,
       const boid_species *species);
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

  const boid_species *species;
  const texture tex;
  const shader *triangle_object_shader;
};

inline boid::boid(const shader *object_shader, double xpos, double ypos,
                  double zpos, const boid_species *species)
    : triangle(object_shader, xpos, ypos, zpos),
      entity(1.0f, glm::sphericalRand(0.5f)), species(species),
      tex(TEXTURE_PATH("diamond.png")), triangle_object_shader(object_shader) {
  this->add_texture(&tex, "texture0");
  this->set_scale(0.5f);
}

inline boid::~boid() {}

inline void boid::update(const std::list<const boid *> &boids,
                         const collider *scene, double deltaTime) {

  glm::vec3 position = this->get_position();

  glm::vec3 sep = separation(boids) * SEP_SCALE;
  glm::vec3 ali = alignment(boids) * ALI_SCALE;
  glm::vec3 coh = cohesion(boids) * COH_SCALE;
  // our addition to alg: a ground separation force
  glm::vec3 ground_sep = velocity * glm::vec3(0., GROUND_SEP_SCALE, 0.) *
                         (float)pow(position.y, -1);
  // another custom addition: a preference for a certain y position
  glm::vec3 pref_y =
      glm::vec3(0., (float)pow(species->pref_y - position.y, 3.f), 0.) *
      PREF_Y_SCALE;

  this->apply_force(sep + ali + coh + ground_sep + pref_y);

  this->evaluate(deltaTime);

  glm::vec3 translation = velocity * (float)deltaTime;

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
}

inline void boid::evaluate(double deltaTime) {
  entity::evaluate(deltaTime);
  if (glm::length(velocity) > species->max_speed) {
    velocity = glm::normalize(velocity) * species->max_speed;
  }
}

inline glm::vec3 boid::separation(const std::list<const boid *> &boids) {
  glm::vec3 steer(0.0f);
  int count = 0;
  for (const boid *other : boids) {
    glm::vec3 diff = this->get_position() - other->get_position();
    float distance = glm::length(diff);
    if (distance > 0 && distance < species->sep_dist) {
      diff = glm::normalize(diff) / distance;
      steer += diff;
      count++;
    }
  }
  if (count > 0) {
    steer /= static_cast<float>(count);
  }
  if (glm::length(steer) > 0) {
    steer = glm::normalize(steer) * species->max_speed - velocity;
    if (glm::length(steer) > MAX_FORCE) {
      steer = glm::normalize(steer) * MAX_FORCE;
    }
  }
  return steer;
}

inline glm::vec3 boid::alignment(const std::list<const boid *> &boids) {
  glm::vec3 sum(0.0f);
  int count = 0;
  for (const boid *other : boids) {
    float distance = glm::length(this->get_position() - other->get_position());
    if (distance > 0 && distance < species->ali_dist) {
      sum += other->get_velocity();
      count++;
    }
  }
  if (count > 0) {
    sum /= static_cast<float>(count);
    sum = glm::normalize(sum) * species->max_speed;
    glm::vec3 steer = sum - velocity;
    if (glm::length(steer) > MAX_FORCE) {
      steer = glm::normalize(steer) * MAX_FORCE;
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
    if (distance > 0 && distance < species->coh_dist) {
      sum += other_position;
      count++;
    }
  }
  if (count > 0) {
    sum /= static_cast<float>(count);
    return glm::normalize(sum - position) * species->max_speed - velocity;
  }
  return glm::vec3(0.0f);
}
