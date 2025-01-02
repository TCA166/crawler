
#pragma once

#include "object.hpp"

/*!
 @brief An object that is a cube
*/
class cube : public object {
  public:
    /*!
     @brief Create a new cube object
     @param object_shader The shader to use for rendering
     @param xpos The x position of the object
     @param ypos The y position of the object
     @param zpos The z position of the object
    */
    cube(const shader *object_shader, double xpos, double ypos, double zpos);
    ~cube();
};
