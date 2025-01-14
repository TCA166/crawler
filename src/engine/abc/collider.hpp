#pragma once

#include "../include.hpp"

/*!
 @brief Interface for objects that can be collided with
*/
class collider {
public:
  /*!
   @brief Check if a point is inside the collider
   @param point The point to check
   @return True if the point is inside the collider
  */
  virtual bool check_point(glm::vec3 point) const = 0;
  /*!
   @brief Check if a line intersects the collider
   @param a The start of the line
   @param b The end of the line
   @return True if the line intersects the collider
  */
  virtual bool check_line(glm::vec3 a, glm::vec3) const = 0;
  virtual ~collider() {}
};
