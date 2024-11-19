
#pragma once

#include "engine/engine.hpp"
#include "physics/entity.hpp"

class game_object : public object, public entity {
    public:
        /*!
         @brief Constructs a game object with a given shader and path to an obj file
         @param object_shader The shader to use for rendering
         @param path The path to the obj file
         @param mass The mass of the object
         @param velocity The velocity of the object
         @param xpos The x position of the object
         @param ypos The y position of the object
         @param zpos The z position of the object
        */
        game_object(const shader* object_shader, const std::string& path, double mass, glm::vec3 velocity, double xpos, double ypos, double zpos);
        virtual ~game_object();
        /*!
         @brief Evaluate the object
         @param delta_time The time since the last evaluation
        */
        virtual void evaluate(double delta_time);
};

/*!
 @brief Calculate the gravitational force between two objects
 @param obj1 The object exerting the gravitational force
 @param obj2 The object experiencing
 @return The gravitational force between the two objects
 @note Naturally you can always invert the force to get the force exerted by obj2 on obj1, but the argument order is important
*/
glm::vec3 calculate_gravity(const game_object* obj1, const game_object* obj2);

/*!
 @brief Resolve the gravitational forces between a list of objects
 @param objects The list of objects to resolve the gravitational forces between
 @note This function will apply the forces to the objects in the list, you must evaluate the objects after calling this function
*/
void resolve_gravity(std::vector<game_object*> objects);
