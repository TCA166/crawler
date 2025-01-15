#pragma once

#include "../renderable/model.hpp"

#include <map>
#include <string>

/*!
 @brief A singleton model loader that manages their initialization
 deinitialization
 @details A single centralized point for loading and storage of models. Actually
 a singleton factory that caches loaded models and retrieves them based on a
 certain key.
 @warning The renderer is responsible for initializing the models through the
 init() method
*/
class model_loader {
private:
  model cube;
  model triangle;
  model wall;
  std::map<std::string, model *> models;
  model_loader();

public:
  /*!
   @brief Retrieves the singleton instance
   @return the loader
  */
  static model_loader &get();
  /*!
   @brief Loads and returns a model, or retrieves it from cache
   @param key the key under which to access the model
   @return the model
  */
  const model *get_model(const std::string &key);
  /*!
   @brief Gets the simple cube model
   @return Cube model
  */
  const model *get_cube() const;
  /*!
   @brief Gets the triangle model
   @return Triangle model
  */
  const model *get_triangle() const;
  /*!
   @brief Gets the wall model
   @return Wall model
  */
  const model *get_wall() const;
  /*!
   @brief Initializes the loaded models
  */
  void init();
  /*!
   @brief Deinitializes the loaded models
  */
  void deinit() const;
};
