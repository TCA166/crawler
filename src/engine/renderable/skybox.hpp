#pragma once

#include "../gl/cubemap.hpp"
#include "../gl/shader.hpp"
#include "object.hpp"

/*!
 @brief A cube with a skybox texture
*/
class skybox : protected object {
private:
  cubemap *skybox_texture;

public:
  /*!
   @brief Create a new skybox object
   @param paths The paths to the skybox textures
  */
  skybox(std::vector<std::string> &paths);
  ~skybox();
  /*!
   @brief Render the skybox
   @param view_projection The view projection matrix
   @param camera_position The position of the camera
   @param lights The lights in the scene
   @param ambient_light The ambient light in the scene
  */
  void render(const camera *target_camera, const shader *current_shader,
              uint32_t tex_off) const;
};
