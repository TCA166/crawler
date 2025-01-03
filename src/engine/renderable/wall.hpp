#pragma once

#include "object.hpp"

class wall : public object {
  public:
    /*!
     @brief Create a new wall object
     @param object_shader The shader to use for rendering
     @param xpos The x position of the object
     @param ypos The y position of the object
     @param zpos The z position of the object
    */
    wall(const shader *object_shader, double xpos, double ypos, double zpos);
    ~wall();
};
