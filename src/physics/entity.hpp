
#pragma once

#include <glm/glm.hpp>

class entity {
    private:
        double mass;
        glm::vec3 momentum;
    public:
        entity(double mass, glm::vec3 momentum);
        ~entity();
        /*!
         @brief Gets the force this entity enacts at another object with a given mass at a distance
         @param distance the distance to the other object
         @param mass the mass of the other objects
         @return the gravitational force enacted
        */
        double get_gravity(double distance, double mass) const;
        /*!
         @brief Applies a given force upon this entity
         @param force the force to apply
        */
        void apply_force(glm::vec3 force);
        /*!
         @brief Gets the mass of the entity
         @return the mass
        */
        double get_mass() const;
        /*!
         @brief Gets the momentum of the entity
         @return the momentum
        */
        glm::vec3 get_momentum() const;
};
