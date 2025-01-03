#pragma once

#include "../gl/cubemap.hpp"
#include "../gl/shader.hpp"
#include "cube.hpp"

class skybox : protected cube {
  private:
    cubemap *skybox_texture;

  public:
    skybox(const shader *skybox_shader, std::vector<std::string> &paths);
    ~skybox();
    void render(const glm::mat4 *view_projection, glm::vec3 camera_position,
                const std::vector<const light *> &lights,
                glm::vec3 ambient_light) const;
    void init();
};
