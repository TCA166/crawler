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
   @param target_camera The camera to render the skybox with
   @param current_shader The shader to render the skybox with
   @param tex_off The offset to start the textures at
  */
  void render(const camera *target_camera, const shader *current_shader,
              uint32_t tex_off) const;
};
