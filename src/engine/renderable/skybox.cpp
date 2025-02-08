#include "skybox.hpp"

#include "../utils/model_loader.hpp"

skybox::skybox(std::vector<std::string> &paths)
    : object(model_loader::get().get_cube(), 0.0, 0.0, 0.0) {
  skybox_texture = new cubemap(paths);
  this->add_texture(skybox_texture, "skybox");
  this->set_scale(100.0);
}

skybox::~skybox() { delete skybox_texture; }

void skybox::render(const camera *target_camera, const shader *current_shader,
                    uint32_t tex_off) const {
  glDepthFunc(GL_LEQUAL);
  object::render(target_camera, current_shader, tex_off);
  glDepthFunc(GL_LESS);
}
