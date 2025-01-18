
#pragma once

#include "object.hpp"

/*!
 @brief A triangle object
 @details A flat 2D triangle object
*/
class triangle : public object {
public:
  /*!
   @brief Constructs a triangle object
   @param object_shader The shader to use for rendering
   @param xpos The x position of the triangle
   @param ypos The y position of the triangle
   @param zpos The z position of the triangle
  */
  triangle(const shader *object_shader, double xpos, double ypos, double zpos);
  ~triangle();

	static const std::vector<float>& get_triangle_data();
};
