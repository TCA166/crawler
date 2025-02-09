
#pragma once

#include "object.hpp"

/*!
 @brief A grass object
 @details A flat 2D triangle object
*/
class grass : public object {
public:
  /*!
   @brief Constructs a grass object
   @param object_shader The shader to use for rendering
   @param xpos The x position of the grass
   @param ypos The y position of the grass
   @param zpos The z position of the grass
  */
  grass( double xpos, double ypos, double zpos);
  ~grass();
};
