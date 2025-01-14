
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
   @brief The front vector of the camera
   @details The vector signifying the direction the camera is facing
   @warning You probably shouldn't modify this directly, use the update_front
   method instead
  */
  glm::vec3 front;
  /*!
   @brief The speed of the camera movement
  */
  float speed;
  /*!
   @brief The look speed of the camera
  */
  float look_speed;
  /*!
   @brief The field of view of the camera
   @note this can be used to zoom in and out
  */
  float fov;
  float yaw, pitch, roll;
  /*!
   @brief Updates the front vector of the camera.
   @details Automatically called when the pitch or yaw is updated.
  */
  void update_front();

public:
  /*!
   @brief Constructs a camera with a given position, speed, fov, look speed,
   pitch and yaw.
   @param position The position of the camera
   @param speed The speed of the camera
   @param fov The field of view of the camera
   @param look_speed The look speed of the camera
   @param pitch The pitch of the camera
   @param yaw The yaw of the camera
   @param roll The roll of the camera
  */
  camera(glm::vec3 position, float speed, float fov, float look_speed,
         float pitch, float yaw, float roll);
  /*!
   @brief Constructs a camera with default values.
   @param position The position of the camera
  */
  camera(glm::vec3 position);
  ~camera();
  /*!
   @brief Move the camera up.
   @param scale The scale to move the camera up by
  */
  void move_up(float scale);
  /*!
   @brief Move the camera left.
   @param scale The scale to move the camera left by
  */
  void move_forward(float scale);
  /*!
   @brief Move the camera right.
   @param scale The scale to move the camera right by
  */
  void move_right(float scale);
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
   @brief Set the speed of the camera
   @param speed The new speed of the camera
  */
  void set_speed(float speed);
  /*!
   @brief Translate the object
   @param translation The translation to apply
  */
  void translate(glm::vec3 translation);
  /*!
   @brief Set the position of the object
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  void set_position(double xpos, double ypos, double zpos);
  /*!
   @brief Rotate the object
   @param xrot The rotation around the x axis
   @param yrot The rotation around the y axis
   @param zrot The rotation around the z axis
  */
  void rotate(double xrot, double yrot, double zrot);
  /*!
   @brief Set the rotation of the object
   @param xrot The rotation around the x axis
   @param yrot The rotation around the y axis
   @param zrot The rotation around the z axis
  */
  void set_rotation(double xrot, double yrot, double zrot);
  glm::vec3 get_front() const;
  glm::vec3 get_rotation() const;
};
