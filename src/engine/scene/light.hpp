
#pragma once

#include "../abc/moveable.hpp"
#include "../abc/view.hpp"
#include "../gl/texture.hpp"

#include <string>

/*!
 @brief a simple invisible light source
*/
class light : public moveable, public view {
private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 color;
  float fov;
  float range;
  unsigned int depthMapFBO;
  unsigned int depthMap;
  glm::vec3 get_front() const;
  glm::vec3 get_up() const;
  bool active;

public:
  /*!
   @brief Constructs a light with a given position, color, fov and
    range
   @param position the position of the light
   @param color the color of the light (rgb)
   @param fov the field of view of the light
   @param range the range of the light
   @param active whether the light should be active
  */
  light(glm::vec3 position, glm::vec3 color, float fov, float range,
        bool active);
  virtual ~light();
  /*!
   @brief Gets the color of the light
   @return the color of the light
  */
  const glm::vec3 &get_color() const;
  /*!
   @brief Gets the position of the light
   @return the position of the light
  */
  const glm::vec3 &get_position() const;
  /*!
   @brief Get the light space matrix
   @return the matrix that transforms coordinates from the world space to the
      light space
  */
  const glm::mat4 get_light_space() const;
  /*!
   @brief Rotates the light
   @param target the target to look at
  */
  void look_at(glm::vec3 target);
  /*!
   @brief Sets the color of the light
   @param color the new color of the light
  */
  void set_color(glm::vec3 color);
  /*!
   @brief Set the position of the object
   @param position The new position of the object
  */
  void set_position(glm::vec3 position);
  /*!
   @brief Translates the light
   @param translation the translation to apply
  */
  void translate(glm::vec3 translation);
  void rotate(glm::vec3 rotation);
  void set_rotation(glm::vec3 rotation);
  /*!
   @brief Binds the depth map of the light
  */
  void bind_view_map() const;
  /*!
   @brief Uses the depth map of the light
   @param texture_unit the texture unit to use
  */
  void use_depth_map(int texture_unit) const;
  /*!
   @brief Gets the depth map of the light
   @return a new texture object based on the depth map of this light
   @warning the caller is responsible for deleting the texture object
  */
  texture *get_view_map() const;
  /*!
   @brief Gets the range of the light
   @return the range of the light
  */
  float get_range() const;
  bool is_active() const;
  void set_active(bool active);
};
