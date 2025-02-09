#pragma once

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"

#define SHOTGUN_SPEED 0.5f
#define MUZZLE_FLASH_LENGTH 0.02f

class shotgun : public object {
public:
  shotgun(light *muzzle_flash, object *flash_sprite, double xpos, double ypos,
          double zpos);
  ~shotgun();
  void look_at(glm::vec3 target);
  void render(const camera *target_camera, const shader *current_shader,
              uint32_t tex_off) const;
  void update(double delta_time);
  bool shoot();

private:
  texture tex, norm;
  object handle;
  light *muzzle_flash;
  object *flash_sprite;
  float last_shot = 0.0f;
};

inline shotgun::shotgun(light *muzzle_flash, object *flash_sprite, double xpos,
                        double ypos, double zpos)
    : object(model_loader::get().get_model(MODEL_PATH("shotgun.obj"), 0), xpos,
             ypos, zpos),
      tex(TEXTURE_PATH("shotgun_base.png")),
      norm(TEXTURE_PATH("shotgun_normal.png")),
      handle(model_loader::get().get_model(MODEL_PATH("shotgun.obj"), 1), 0.,
             0., 0.),
      muzzle_flash(muzzle_flash), flash_sprite(flash_sprite) {
  this->add_texture(&tex, "texture0");
  this->add_texture(&norm, "normal0");
}

inline shotgun::~shotgun() {}

inline void shotgun::look_at(glm::vec3 target) {
  glm::vec3 direction = glm::normalize(target - get_position());
  set_rotation(
      glm::vec3(0.f, -atan2(direction.z, direction.x), asin(direction.y)));
}

inline void shotgun::render(const camera *, const shader *current_shader,
                            uint32_t tex_off) const {
  size_t tex_i = tex_off;
  for (const auto &pair : textures) {
    pair.second->set_active_texture(current_shader, tex_i, pair.first);
    tex_i++;
  }

  glm::mat4 main_model_matrix = get_model_matrix();

  current_shader->apply_uniform_mat4(main_model_matrix, "model");

  this->draw();

  current_shader->apply_uniform_mat4(
      glm::translate(
          main_model_matrix * handle.get_model_matrix(),
          glm::vec3(-sin(last_shot * M_PI / SHOTGUN_SPEED) * 0.09, 0., 0.)),
      "model");

  handle.draw();

  // unbind textures
  for (size_t i = tex_off; i < tex_i; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

inline void shotgun::update(double delta_time) {
  if (last_shot > 0.0f) {
    last_shot -= delta_time;
  }
  if (last_shot < SHOTGUN_SPEED - MUZZLE_FLASH_LENGTH) {
    muzzle_flash->set_active(false);
    flash_sprite->set_active(false);
  }
}

inline bool shotgun::shoot() {
  if (last_shot <= 0.f) {
    last_shot = SHOTGUN_SPEED;
    muzzle_flash->set_rotation(-rot);
    muzzle_flash->set_active(true);
    glm::mat4 model_matrix = get_model_matrix();
    glm::vec3 muzzle_position =
        glm::vec3(model_matrix * glm::vec4(0.5f, 0.2f, 0.f, 1.0f));
    flash_sprite->set_position(muzzle_position);
    rot.y += glm::radians(90.f);
    flash_sprite->set_rotation(rot);
    flash_sprite->set_active(true);
    return true;
  } else {
    return false;
  }
}
