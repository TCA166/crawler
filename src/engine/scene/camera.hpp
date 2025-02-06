
#pragma once

#include "../abc/moveable.hpp"
#include "../include.hpp"

/*!
 @brief Camera class to handle camera movement.
*/
class camera : public moveable {
private:
  /*!
   @brief The position of the camera
   @details Offset from the origin
  */
  glm::vec3 position;
  /*!
   @brief The look speed of the camera
  */
  float look_speed;
  /*!
   @brief The field of view of the camera
   @note this can be used to zoom in and out
  */
  float fov;
  glm::vec3 rotation;

public:
  /*!
   @brief Constructs a camera with a given position, speed, fov, look speed,
   pitch and yaw.
   @param position The position of the camera
   @param fov The field of view of the camera
   @param look_speed The look speed of the camera
   @param pitch The pitch of the camera
   @param yaw The yaw of the camera
   @param roll The roll of the camera
  */
  camera(glm::vec3 position, float fov, float look_speed, float pitch,
         float yaw, float roll);
  /*!
   @brief Constructs a camera with default values.
   @param position The position of the camera
  */
  camera(glm::vec3 position);
  ~camera();
  /*!
   @brief Zooms in the camera by adjusting the fov
   @param value The value to zoom in by
  */
  void zoom(float value);
  /*!
   @brief Get the view matrix of the camera
   @return The view matrix of the camera
  */
  glm::mat4 get_view_matrix() const;
  /*!
   @brief Get the projection matrix of the camera
   @param aspect_ratio The aspect ratio of the window
   @return The projection matrix of the camera
  */
  glm::mat4 get_projection_matrix(float aspect_ratio) const;
  /*!
   @brief Get the position of the camera
   @return The position of the camera
  */
  glm::vec3 get_position() const;
  /*!
   @brief Translate the object
   @param translation The translation to apply
  */
  void translate(glm::vec3 translation);
  /*!
   @brief Set the position of the object
   @param position The new position of the object
  */
  void set_position(glm::vec3 position);
  /*!
   @brief Rotate the object
   @param xrot The rotation around the x axis
   @param yrot The rotation around the y axis
   @param zrot The rotation around the z axis
  */
  void rotate(glm::vec3 rotation);
  /*!
   @brief Set the rotation of the object
   @param rotation The new rotation of the object
  */
  void set_rotation(glm::vec3 rotation);
  /*!
   @brief Get the front vector of the camera
   @return The front vector of the camera
  */
  glm::vec3 get_front() const;
  /*!
   @brief Get the rotation of the camera
   @return The rotation of the camera
  */
  glm::vec3 get_rotation() const;
  /*!
   @brief Get the up vector of the camera
   @return The up vector
  */
  glm::vec3 get_up() const;
  /*!
   @brief Get the right vector of the camera
   @return The right vector
  */
  glm::vec3 get_right() const;
};
