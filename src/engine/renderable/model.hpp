#pragma once

#include "../include.hpp"

#include <string>
#include <vector>

#define MODEL_LINE(x, y, z, tx, ty, nx, ny, nz, tgx, tgy, tgz, btx, bty, btz)  \
  x, y, z, tx, ty, nx, ny, nz, tgx, tgy, tgz, btx, bty, btz

#define MODEL_LINE_SIZE 14

class instanced_model;

/*!
 @brief A collection of OpenGL entities necessary for things to get drawn
*/
class model {
protected:
  /*!
   @brief Hidden constructor creating an empty model
  */
  model() = default;
  /*!
   @brief the data of the object (vertices, texture coordinates)
  */
  std::vector<float> data;
  /*!
   @brief the indices of the object
  */
  std::vector<unsigned int> indices;
  ///@{
  /*!
   @brief The bounds of the object
  */
  glm::vec3 bounds, negbounds;
  ///@}
  GLuint VAO, VBO, EBO;

public:
  /*!
   @brief Create a new model based on provided data
   @param data the data to use. Assumed to match MODEL_LINE format
   @param indices the indices pointing into the data
   @param bounds the upper bounds for the model
   @param negbounds the lower bounds for the model
  */
  model(const std::vector<float> &data,
        const std::vector<unsigned int> &indices, glm::vec3 bounds,
        glm::vec3 negbounds);
#ifndef STATIC_ASSETS
  /*!
   @brief Create a new model using the obj model at the provided path
   @param path the path pointing to the model
   @param mesh_index the index of the mesh to use
  */
  model(const std::string &path, uint32_t mesh_index = 0);
#endif
  ~model();
  /*!
   @brief Initializes the model within the OpenGL context
  */
  virtual void init();
  /*!
   @brief Undoes the OpenGL initialization
  */
  virtual void deinit() const;
  /*!
   @brief Gets the upper model bounds
   @return The upper model bounds
  */
  glm::vec3 get_bounds() const;
  /*!
   @brief Gets the lower model bounds
   @return The lower model bounds
  */
  glm::vec3 get_negbounds() const;
  /*!
   @brief Draws the model onto the viewport
  */
  virtual void draw() const;
  instanced_model *get_instanced(const std::vector<glm::vec3> &instances) const;
};

class instanced_model : public model {
private:
  GLuint instanceVBO;
  const std::vector<glm::vec3> instances;

public:
  instanced_model(const std::vector<float> &data,
                  const std::vector<unsigned int> &indices, glm::vec3 bounds,
                  glm::vec3 negbounds, const std::vector<glm::vec3> &instances);
  ~instanced_model();
  void init() override;
  void deinit() const override;
  void draw() const override;
};
