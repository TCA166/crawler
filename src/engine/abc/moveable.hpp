
#pragma once

#include "../include.hpp"

/*!
 @brief Interface for objects that can be moved.
 @warning The rotation should be expressed in radians, and the translation in
 units.
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
   @param position The new position of the object
  */
  virtual void set_position(glm::vec3 position) = 0;
  /*!
   @brief Rotate the object
   @param rotation The rotation to apply
  */
  virtual void rotate(glm::vec3 rotation) = 0;
  /*!
   @brief Set the rotation of the object
   @param rotation The new rotation of the object
  */
  virtual void set_rotation(glm::vec3 rotation) = 0;
  virtual ~moveable() {}
};
