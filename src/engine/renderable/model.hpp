#pragma once

#include "../include.hpp"

#include <string>
#include <vector>

#define MODEL_LINE(x, y, z, tx, ty) x, y, z, tx, ty

#define MODEL_LINE_SIZE 5

/*!
 @brief A collection of OpenGL entities necessary for things to get drawn
*/
class model {
private:
  /*!
   @brief the data of the object (vertices, texture coordinates)
  */
  std::vector<float> data;
  /*!
   @brief the indices of the object
  */
  std::vector<unsigned int> indices;
  GLuint VAO, VBO, EBO;
  ///@{
  /*!
   @brief The bounds of the object
  */
  glm::vec3 bounds, negbounds;
  ///@}

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
  */
  model(const std::string &path);
#endif
  ~model();
  /*!
   @brief Initializes the model within the OpenGL context
  */
  void init();
  /*!
   @brief Undoes the OpenGL initialization
  */
  void deinit() const;
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
  void draw() const;
};
