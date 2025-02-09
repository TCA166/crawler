
#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include "../abc/collider.hpp"
#include "../abc/moveable.hpp"
#include "../gl/shader.hpp"
#include "../gl/texture.hpp"
#include "../scene/camera.hpp"
#include "../scene/light.hpp"
#include "model.hpp"

/*!
 @brief Object class to handle rendering of objects.
*/
class object : public moveable, public collider {
protected:
  /*!
   @brief The scale of the object
  */
  glm::vec3 scale;
  /*!
   @brief The rotation of the object
  */
  glm::vec3 rot;
  /*!
   @brief The model of the object
  */
  const model *object_model;
  /*!
   @brief Map of name->texture
   @details This is used to provide the shader texture names, with map keys as
    the texture names and the values as the textures
  */
  std::unordered_map<std::string, const texture *> textures;
  /*!
   @brief The number of textures
  */
  unsigned int texture_count;
  /*!
   @brief The children of the object, objects that are moved with the object
  */
  std::list<moveable *> children;
  /*!
   @brief The parents of the object, objects that move the object
  */
  std::list<object *> parents;
  /*!
   @brief The position of the object
  */
  glm::vec3 position;
  /*!
   @brief Gets the positive bounds of the object
   @return A vector with 1 points on each axis representing the largest extent
   of the model on that axis
  */
  glm::vec3 get_bounds() const;
  /*!
   @brief Gets the negative bounds of the object
   @return A vector with 1 point on each axis represeting the largest extent in
   the opposite direction
  */
  glm::vec3 get_negbounds() const;
  bool active;

public:
  /*!
   @brief Constructs an object with a given shader and model
   @param object_model the model to use for this object
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  object(const model *object_model, double xpos, double ypos, double zpos);
  virtual ~object();
  /*!
   @brief Render the object
   @param target_camera The camera to render the object with
   @param current_shader The shader to render the object with
   @param tex_off The offset to start the textures at
  */
  virtual void render(const camera *target_camera, const shader *current_shader,
                      uint32_t tex_off) const;
  /*!
   @brief Draws the object onto the viewport
  */
  virtual void draw() const;
  /*!
   @brief Add a texture to the object
   @param tex The texture to add
   @param name the name under which the texture should be added and accessed
  */
  void add_texture(const texture *tex, std::string name);
  /*!
   @brief Set the position of the object
   @param position The position to set
  */
  void set_position(glm::vec3 position);
  /*!
   @brief Set the scale of the object
   @param scalex The x scale of the object
   @param scaley The y scale of the object
   @param scalez The z scale of the object
  */
  void set_scale(float scalex, float scaley, float scalez);
  /*!
   @brief Set the rotation of the object
   @param scale The scale to apply
  */
  void set_scale(float scale);
  /*!
   @brief Set the rotation of the object in radians
   @param rotation The rotation to apply
  */
  void set_rotation(glm::vec3 rotation);
  /*!
   @brief Rotate the object in radians
   @param rotation The rotation to apply
  */
  void rotate(glm::vec3 rotation);
  /*!
   @brief Translate the object
   @param translation The translation to apply
  */
  void translate(glm::vec3 translation);
  /*!
   @brief Get the position of the object
   @return The position of the object
  */
  glm::vec3 get_position() const;
  /*!
   @brief Adds a child to the object
   @param child The child to add
   @warning The child will not add this object as a parent
  */
  void add_child(moveable *child);
  /*!
   @brief Remove a child from the object
   @param child The child to remove
  */
  void remove_child(const moveable *child);
  /*!
   @brief Add a parent to the object
   @param parent The parent to add
   @note Adds this object as a child of the parent
  */
  void add_parent(object *parent);
  /*!
   @brief Get the model matrix of the object
   @return The model matrix of the object
  */
  glm::mat4 get_model_matrix() const;
  /*!
   @brief Check if a point is within the bounds of the object
   @param point The point to check
   @return True if the point is within the bounds of the object
  */
  bool check_point(glm::vec3 point) const;
  /*!
   @brief Check if a line collides with the object
   @param a The start of the line
   @param b The end of the line
   @return True if the line collides with the object
  */
  bool check_line(glm::vec3 a, glm::vec3 b) const;
  bool is_active() const;
  void set_active(bool active);
};
