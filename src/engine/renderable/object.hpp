
#pragma once

#include <map>
#include <vector>

#include "../abc/moveable.hpp"
#include "../gl/shader.hpp"
#include "../gl/texture.hpp"
#include "../scene/camera.hpp"
#include "../scene/light.hpp"

/*!
 @brief Object class to handle rendering of objects.
*/
class object : public moveable {
private:
  const shader *object_shader;
  /*!
   @brief the data of the object (vertices, normals, texture coordinates,
   tangents and bitangents)
  */
  std::vector<float> data;
  /*!
   @brief the indices of the object
  */
  std::vector<unsigned int> indices;
  GLuint VAO, VBO, EBO;
  glm::vec3 scale, rot;
  unsigned int vertex_count;

protected:
  /*!
   @brief Map of name->texture
  */
  std::map<std::string, const texture *> textures;
  /*!
   @brief The number of textures
  */
  unsigned int texture_count;
  /*!
   @brief The children of the object, objects that are moved with the object
  */
  std::vector<moveable *> children;
  ///@{
  /*!
   @brief The bounds of the object
  */
  float xbound, xnegbound, ybound, ynegbound, zbound, znegbound;
  ///@}
  double xpos, ypos, zpos;

public:
  /*!
   @brief Constructs an object with a given shader and path to an obj file
   @param object_shader The shader to use for rendering
   @param path The path to the obj file
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  object(const shader *object_shader, const std::string &path, double xpos,
         double ypos, double zpos);
  /*!
   @brief Constructs an object with a given shader and data
   @param object_shader The shader to use for rendering
   @param data The data of the object
   @param indices The indices of the object
   @param xbound The x bound of the object
   @param xnegbound The negative x bound of the object
   @param ybound The y bound of the object
   @param ynegbound The negative y bound of the object
   @param zbound The z bound of the object
   @param znegbound The negative z bound of the object
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  object(const shader *object_shader, const std::vector<float> &data,
         const std::vector<unsigned int> &indices, float xbound,
         float xnegbound, float ybound, float ynegbound, float zbound,
         float znegbound, double xpos, double ypos, double zpos);
  virtual ~object();
  /*!
   @brief Initialize the object
   @details Initializes the object by creating the VAO, VBO and EBO
   @warning This function must be called before rendering the object, and
   should only be called once per context
  */
  void init();
  /*!
   @brief Render the object
   @param target_camera The camera to render the object with
   @param aspect_ratio The aspect ratio of the window
   @param lights The lights to render the object with
   @param ambient The ambient light to render the object with
  */
  void render(const camera *target_camera, float aspect_ratio,
              const std::vector<const light *> &lights,
              glm::vec3 ambient) const;
  /*!
   @brief Render the object, including drawing the object onto the viewport
   @param viewProjection The view projection matrix to render the object with
   @param viewPos The position of the camera
   @param lights The lights to render the object with
   @param ambient The ambient light to render the object with
  */
  virtual void render(const glm::mat4 *viewProjection, glm::vec3 viewPos,
                      const std::vector<const light *> &lights,
                      glm::vec3 ambient) const;
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
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  void set_position(double xpos, double ypos, double zpos);
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
   @param xrot The x rotation of the object in radians
   @param yrot The y rotation of the object in radians
   @param zrot The z rotation of the object in radians
  */
  void set_rotation(double xrot, double yrot, double zrot);
  /*!
   @brief Rotate the object in radians
   @param xrot The delta x rotation
   @param yrot The delta y rotation
   @param zrot The delta z rotation
  */
  void rotate(double xrot, double yrot, double zrot);
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
  */
  void add_child(moveable *child);
  /*!
   @brief Get the model matrix of the object
   @return The model matrix of the object
  */
  glm::mat4 get_model_matrix() const;
  /*!
   @brief Check if a point is within the bounds of the object
   @param point The point to check
   @return Whether the point is within the bounds of the object
  */
  bool check_bounds(glm::vec3 point) const;
  /*!
   @brief Check if a line collides with the object
   @param a The start of the line
   @param b The end of the line
   @return Whether the line collides with the object
  */
  bool check_bounds(glm::vec3 a, glm::vec3 b) const;
};
