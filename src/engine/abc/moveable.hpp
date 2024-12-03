
#pragma once

#include "../include.hpp"

/*!
 @brief Interface(abstract class) for objects that can be moved
*/
class moveable {
    public:
        /*!
         @brief Translate the object
         @param translation The translation to apply
        */
        virtual void translate(glm::vec3 translation) = 0;
        /*!
         @brief Set the position of the object
         @param xpos The x position of the object
         @param ypos The y position of the object
         @param zpos The z position of the object
        */
        virtual void set_position(double xpos, double ypos, double zpos) = 0;
        virtual ~moveable() {}
};
