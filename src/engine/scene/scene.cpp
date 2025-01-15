#include "scene.hpp"

#include "../settings.hpp"

#include <iostream>

scene::scene(glm::vec3 ambient_light, glm::vec3 background_color)
    : ambient_light(ambient_light), background_color(background_color),
      sky(nullptr) {}

scene::~scene() {}

void scene::set_skybox(skybox *sky) { this->sky = sky; }

void scene::add_object(object *obj) { objects.push_back(obj); }

void scene::remove_object(const object *obj) {
  objects.remove(const_cast<object *>(obj));
}

void scene::remove_light(const light *lght) {
  lights.remove(const_cast<light *>(lght));
}

void scene::add_light(light *light) { lights.push_back(light); }

void scene::init() {
  light_pass_shader = new shader(SHADER_PATH("light_pass.vert"),
                                 SHADER_PATH("light_pass.frag"));
  initialized = true;
}

void scene::render(const camera *target_camera, float aspect_ratio) {
  glClearColor(background_color.r, background_color.g, background_color.b,
               1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 projection = target_camera->get_projection_matrix(aspect_ratio);
  glm::mat4 view = target_camera->get_view_matrix();
  if (sky != nullptr) {
    // special projection matrix that removes the translation
    glm::mat4 viewProjection = projection * glm::mat4(glm::mat3(view));
    sky->render(&viewProjection, target_camera->get_position(),
                (const std::list<const light *> &)lights, ambient_light);
  }
  glm::mat4 viewProjection = projection * view;
  for (const object *obj : objects) {
    obj->render(&viewProjection, target_camera->get_position(),
                (const std::list<const light *> &)lights, ambient_light);
  }
}

void scene::shadow_pass() const {
  // resize the viewport to the shadow resolution
  glViewport(0, 0, SHADOW_RES, SHADOW_RES);
  glCullFace(GL_FRONT);
  // activate the super simple shader for the shadow pass
  light_pass_shader->use();
  for (const light *lght : lights) {
    lght->bind_view_map(); // activate the framebuffer
    light_pass_shader->apply_uniform_mat4(lght->get_light_space(),
                                          "lightSpaceMatrix");
    // draw all the objects
    for (object *obj : objects) {
      light_pass_shader->apply_uniform_mat4(obj->get_model_matrix(), "model");
      obj->draw();
    }
    // cleanup
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  glUseProgram(0);
  glCullFace(GL_BACK);
}

void scene::main(camera *) {
  while (!should_close) {
    // do nothing
  }
}

void scene::scroll_callback(double, double, camera *) {
  // do nothing
}

void scene::key_callback(int, int, int, int, camera *) {
  // do nothing
}

void scene::mouse_button_callback(int, int, int, camera *) {
  // do nothing
}

void scene::mouse_callback(double, double, camera *) {
  // do nothing
}

void scene::close_callback() { should_close = true; }

bool scene::get_should_close() const { return should_close; }

bool scene::check_point(glm::vec3 point) const {
  for (const object *obj : objects) {
    if (obj->check_point(point)) {
      return true;
    }
  }
  return false;
}

bool scene::check_line(glm::vec3 a, glm::vec3 b) const {
  for (const object *obj : objects) {
    if (obj->check_line(a, b)) {
      return true;
    }
  }
  return false;
}
