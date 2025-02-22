
#pragma once

#ifndef WASM
#include <glm/ext.hpp>
#else
#include <ext.hpp>
#endif

/*!
 @brief An entity in the physics engine
 @details Doesn't necessarily need to be rendered, but has physical properties
 like mass, velocity, and force
*/
class entity {
private:
  glm::vec3 force_accumulator;

protected:
  /*!
   @brief The mass of the entity
  */
  float mass;
  /*!
   @brief The velocity of the entity
  */
  glm::vec3 velocity;

public:
  /*!
   @brief Constructs an entity with a given mass and velocity
   @param mass the mass of the entity
   @param velocity the velocity of the entity
  */
  entity(float mass, glm::vec3 velocity);
  virtual ~entity();
  /*!
   @brief Gets the force this entity enacts at another object with a given
   mass at a distance
   @param distance the distance to the other object
   @param mass the mass of the other objects
   @return the gravitational force enacted
  */
  float get_gravity(float distance, float mass) const;
  /*!
   @brief Applies a given force upon this entity
   @param force the force to apply
  */
  void apply_force(glm::vec3 force);
  /*!
   @brief Gets the mass of the entity
   @return the mass
  */
  float get_mass() const;
  /*!
   @brief Gets the momentum of the entity
   @return the momentum
  */
  glm::vec3 get_velocity() const;
  /*!
   @brief Evaluates the entity
   @param delta_time the time since the last evaluation
  */
  virtual void evaluate(double delta_time);
};
