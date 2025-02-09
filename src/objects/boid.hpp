#pragma once

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"
#include "../physics/entity.hpp"

#define SEP_SCALE 4.0f
#define ALI_SCALE 1.0f
#define COH_SCALE 0.5f
#define PREF_Y_SCALE 0.01f
#define DISLIKE_SCALE 2.0f

#define MAX_FORCE 1.0f

#define MAX_X 10.0f
#define MAX_Y 10.0f
#define MAX_Z 10.0f
#define MIN_X -10.0f
#define MIN_Y -10.0f
#define MIN_Z -10.0f

/*!
 @brief A species of boid, with parameters for flocking
*/
struct boid_species {
  /*!
   @brief A unique identifier for the species
  */
  uint32_t id;
  /*!
   @brief The minimum acceptable distance between boids
  */
  float sep_dist;
  /*!
   @brief The distance within which to align to other boids
  */
  float ali_dist;
  /*!
   @brief The distance within which to cohere with other boids
  */
  float coh_dist;
  /*!
   @brief The maximum speed of the boid
  */
  float max_speed;
  /*!
   @brief The y position the boid prefers
  */
  float pref_y;
};

/*!
 @brief A boid object
 @details A boid object that can be used in a flocking simulation
*/
class boid : public object, protected entity {
public:
  /*!
   @brief Constructs a boid object
   @param tex The texture of the boid
   @param norm The normal map of the boid
   @param xpos The x position of the boid
   @param ypos The y position of the boid
   @param zpos The z position of the boid
   @param species The species of the boid
  */
  boid(const texture *tex, const texture *norm, double xpos, double ypos,
       double zpos, const boid_species *species);
  ~boid();

  /*!
   @brief Updates the boid, including physics and flocking
   @param boids The boids to flock with
   @param scene The collider to check collisions against
   @param deltaTime The time since the last update
  */
  void update(const std::list<const boid *> &boids, const collider *scene,
              double deltaTime);

  /*!
   @brief Returns the species id of the boid
   @return The species id of the boid
  */
  uint32_t get_species_id() const;

private:
  void evaluate(double deltaTime);

  const boid_species *species;
};

inline boid::boid(const texture *tex, const texture *norm, double xpos,
                  double ypos, double zpos, const boid_species *species)
    : object(model_loader::get().get_triangle(), xpos, ypos, zpos),
      entity(1.0f, glm::sphericalRand(0.5f)), species(species) {
  this->add_texture(tex, "texture0");
  this->add_texture(norm, "normal0");
  this->set_scale(0.5f);
}

inline boid::~boid() {}

inline void boid::update(const std::list<const boid *> &boids,
                         const collider *scene, double deltaTime) {

  glm::vec3 position = this->get_position();
  //fprintf(stderr, "Position: %f %f %f\n", position.x, position.y, position.z);

  glm::vec3 steer(0.0f); // steer accumulator
  {
    uint32_t count = 0;
    for (const boid *other : boids) {
      // sanity check
      if (other == this) {
        continue;
      }
      glm::vec3 diff = position - other->get_position();
      float distance = glm::length(diff);
      if (distance == 0.0f) {
        //fprintf(stderr, "Distance is zero\n");
        continue;
      }
      if (other->get_species_id() == species->id) { // if species match
        // we add alignment speed if the boid is close enough
        if (distance < species->ali_dist) {
          steer += other->get_velocity() * ALI_SCALE;
          count++;
        }
        // we add cohesion force if the boid is close enough
        if (distance < species->coh_dist) {
          steer += other->get_position() * COH_SCALE;
          count++;
        }
      } else {
        distance /= DISLIKE_SCALE;
      }
      if (distance < species->sep_dist) {
        steer += (glm::normalize(diff) / distance) * SEP_SCALE;
        count++;
      }

      count++;
    }
    if (count > 0) {
      steer /= count;
      if (glm::length(steer) > 0.0f) {
        steer = glm::normalize(steer) * species->max_speed - velocity;
        if (glm::length(steer) > MAX_FORCE) {
          steer = glm::normalize(steer) * MAX_FORCE;
        }
      } else {
        //fprintf(stderr, "Steer length is zero\n");
      }
    }
  }

  // custom addition: a preference for a certain y position
  float pref_y_value = pow(species->pref_y - position.y, 3.f);
  pref_y_value =
      glm::clamp(pref_y_value, -1000.0f, 1000.0f); // Clamping the value
  glm::vec3 pref_y = glm::vec3(0., pref_y_value, 0.) * PREF_Y_SCALE;

  this->apply_force(steer + pref_y);

  // Random perturbation
  glm::vec3 random_perturbation = glm::sphericalRand(0.1f);
  this->apply_force(random_perturbation);

   // Force for centre attraction
  glm::vec3 center(0.0f, 0.0f, 0.0f);
  glm::vec3 to_center = center - position;
  glm::vec3 center_force = glm::normalize(to_center) * 0.01f;
  this->apply_force(center_force);

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

  if (target.x <= MIN_X || target.x >= MAX_X) {
      this->velocity.x = -this->velocity.x;
    target.x = glm::clamp(target.x, MIN_X, MAX_X);
  }

  if (target.y <= MIN_Y || target.y >= MAX_Y) {
    this->velocity.y = -this->velocity.y;
    target.y = glm::clamp(target.y, MIN_Y, MAX_Y);
  }

  if (target.z <= MIN_Z || target.z >= MAX_Z) {
    this->velocity.z = -this->velocity.z;
    target.z = glm::clamp(target.z, MIN_Z, MAX_Z);
  }

    this->set_position(target);
  }
  // Log the final position
  position = this->get_position();

}
inline uint32_t boid::get_species_id() const { return species->id; }

inline void boid::evaluate(double deltaTime) { entity::evaluate(deltaTime); }
