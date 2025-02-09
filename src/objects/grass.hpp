#pragma once

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"

class grass : public object {
public:
  /*!
   @brief Constructs a new leaves object
   @param tex The texture to use for the leaves
   @param points The points to place the leaves at
  */
  grass(const texture *tex, const std::vector<glm::vec3> &points);
  ~grass();
  void render(const camera *target_camera, const shader *current_shader,
              uint32_t tex_off) const;

private:
  const texture *tex;
};

inline grass::grass(const texture *tex, const std::vector<glm::vec3> &points)
    : object(model_loader::get().get_wall()->get_instanced(points), 0.f, 0.f,
             0.f),
      tex(tex) {
  set_scale(2.f, 1.f, 1.f);
}

inline grass::~grass() {}

inline void grass::render(const camera *, const shader *current_shader,
                          uint32_t tex_off) const {
  current_shader->apply_uniform_mat4(get_model_matrix(), "model");
  tex->set_active_texture(current_shader, tex_off, "texture0");
  draw();
  glActiveTexture(GL_TEXTURE0 + tex_off);
  glBindTexture(GL_TEXTURE_2D, 0);
}
