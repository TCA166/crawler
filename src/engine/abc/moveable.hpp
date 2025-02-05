
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
  /*!
   @brief Rotate the object
   @param xrot The rotation around the x axis in radians
   @param yrot The rotation around the y axis in radians
   @param zrot The rotation around the z axis in radians
  */
  virtual void rotate(double xrot, double yrot, double zrot) = 0;
  /*!
   @brief Set the rotation of the object
   @param xrot The rotation around the x axis in radians
   @param yrot The rotation around the y axis in radians
   @param zrot The rotation around the z axis in radians
  */
  virtual void set_rotation(double xrot, double yrot, double zrot) = 0;
  virtual ~moveable() {}
};
