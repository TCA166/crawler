#pragma once

#include "../engine/engine.hpp"
#include "../physics/entity.hpp"

#define SEP_SCALE 4.0f
#define ALI_SCALE 1.0f
#define COH_SCALE 0.5f
#define PREF_Y_SCALE 0.01f
#define DISLIKE_SCALE 2.0f

#define MAX_FORCE 1.0f

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
class boid : public triangle, protected entity {
public:
  /*!
   @brief Constructs a boid object
   @param object_shader The shader to use for rendering
   @param tex The texture of the boid
   @param norm The normal map of the boid
   @param xpos The x position of the boid
   @param ypos The y position of the boid
   @param zpos The z position of the boid
   @param species The species of the boid
  */
  boid(const shader *object_shader, const texture *tex, const texture *norm,
       double xpos, double ypos, double zpos, const boid_species *species);
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
  const shader *triangle_object_shader;
};

inline boid::boid(const shader *object_shader, const texture *tex,
                  const texture *norm, double xpos, double ypos, double zpos,
                  const boid_species *species)
    : triangle(object_shader, xpos, ypos, zpos),
      entity(1.0f, glm::sphericalRand(0.5f)), species(species),
      triangle_object_shader(object_shader) {
  this->add_texture(tex, "texture0");
  this->add_texture(norm, "normal0");
  this->set_scale(0.5f);
}

inline boid::~boid() {}

inline void boid::update(const std::list<const boid *> &boids,
                         const collider *scene, double deltaTime) {

  glm::vec3 position = this->get_position();

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
      steer = glm::normalize(steer) * species->max_speed - velocity;
      if (glm::length(steer) > MAX_FORCE) {
        steer = glm::normalize(steer) * MAX_FORCE;
      }
    }
  }

  // custom addition: a preference for a certain y position
  glm::vec3 pref_y =
      glm::vec3(0., (float)pow(species->pref_y - position.y, 3.f), 0.) *
      PREF_Y_SCALE;

  this->apply_force(steer + pref_y);

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
    this->set_position(target);
  }
}

inline uint32_t boid::get_species_id() const { return species->id; }

inline void boid::evaluate(double deltaTime) { entity::evaluate(deltaTime); }
