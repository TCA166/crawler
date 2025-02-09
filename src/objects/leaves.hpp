#pragma once

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"
#include "../engine/utils/noise.hpp"

/*!
 @brief A collection of leaves. Rendered through instancing
 @details The leaves are a set of instanced sprites that are rendered through
 instancing. Through this method we can render a large number of leaves with
  minimal performance impact
 */
class leaves : public object {
public:
  /*!
   @brief Constructs a new leaves object
   @param tex The texture to use for the leaves
   @param points The points to place the leaves at
  */
  leaves(const texture *tex, const std::vector<glm::vec3> &points);
  ~leaves();
  void render(const camera *target_camera, const shader *current_shader,
              uint32_t tex_off) const;

private:
  const texture *tex;
};

inline leaves::leaves(const texture *tex, const std::vector<glm::vec3> &points)
    : object(model_loader::get().get_wall()->get_instanced(points), 0.f, 0.f,
             0.f),
      tex(tex) {}

inline leaves::~leaves() {}

inline void leaves::render(const camera *, const shader *current_shader,
                           uint32_t tex_off) const {
  current_shader->apply_uniform_mat4(get_model_matrix(), "model");
  tex->set_active_texture(current_shader, tex_off, "leafTexture");
  draw();
  glActiveTexture(GL_TEXTURE0 + tex_off);
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline texture *create_random_leaf_texture(uint32_t size,
                                           uint8_t color_variance) {
  uint8_t *leaf_data = new uint8_t[size * size * 4];
  float radius = size / 2;
  glm::vec2 image_center(radius, radius);
  for (size_t x = 0; x < size; x++) {
    for (size_t y = 0; y < size; y++) {
      size_t i = (x + y * size) * 4;
      float distance = glm::distance(glm::vec2(x, y), image_center);
      if (distance > radius) {
        leaf_data[i + 3] = 0;
        continue; // outside the circle, make it transparent
      }
      leaf_data[i] = 0;
      leaf_data[i + 1] =
          (255 - color_variance) + glm::linearRand((uint8_t)0, color_variance);
      leaf_data[i + 2] = 0;
      float noise_val = noise(x, y) + 1.f * 128.f;
      leaf_data[i + 3] = (uint8_t)noise_val; // alpha
    }
  }
  image_t leaf_image = {leaf_data, size, size, 4};
  return new texture(&leaf_image);
}
