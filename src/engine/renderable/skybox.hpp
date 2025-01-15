#pragma once

#include "../gl/cubemap.hpp"
#include "../gl/shader.hpp"
#include "cube.hpp"

/*!
 @brief A cube with a skybox texture
*/
class skybox : protected cube {
private:
  cubemap *skybox_texture;

public:
  /*!
   @brief Create a new skybox object
   @param skybox_shader The shader to use for rendering
   @param paths The paths to the skybox textures
  */
  skybox(const shader *skybox_shader, std::vector<std::string> &paths);
  ~skybox();
  /*!
   @brief Render the skybox
   @param view_projection The view projection matrix
   @param camera_position The position of the camera
   @param lights The lights in the scene
   @param ambient_light The ambient light in the scene
  */
  void render(const glm::mat4 *view_projection, glm::vec3 camera_position,
              const std::list<const light *> &lights,
              glm::vec3 ambient_light) const;
  /*!
   @brief Initialize the skybox
  */
  void init();
};
