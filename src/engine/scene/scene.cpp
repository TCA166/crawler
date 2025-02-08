#include "scene.hpp"

#include "../settings.hpp"

#include <iostream>

scene::scene(glm::vec3 ambient_light, glm::vec3 background_color)
    : ambient_light(ambient_light), background_color(background_color),
      sky(nullptr), current_time(glfwGetTime()), delta_time(0.0) {}

scene::~scene() {}

void scene::set_skybox(const shader *skybox_shader, skybox *sky) {
  this->skybox_shader = skybox_shader;
  this->sky = sky;
}

void scene::add_object(const shader *target_shader, const object *obj) {
  objects[target_shader].push_back(obj);
}

void scene::remove_object(const object *obj) {
  for (auto &pair : objects) {
    pair.second.remove(obj);
  }
}

void scene::remove_light(const light *lght) {
  lights.remove(const_cast<light *>(lght));
}

void scene::add_light(light *light) { lights.push_back(light); }

void scene::add_collider(const collider *collider) {
  colliders.push_back(collider);
}

void scene::init(camera *) {
  light_pass_shader = new shader(SHADER_PATH("light_pass.vert"),
                                 SHADER_PATH("light_pass.frag"));
  initialized = true;
}

void scene::clear() const {
  glClearColor(background_color.r, background_color.g, background_color.b,
               1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void scene::render(const camera &target_camera, uint16_t width,
                   uint16_t height) {
  clear();
  float aspect_ratio = (float)width / (float)height;
  glm::mat4 projection = target_camera.get_projection_matrix(aspect_ratio);
  glm::mat4 view = target_camera.get_view_matrix();
  if (sky != nullptr) {
    // special projection matrix that removes the translation
    glm::mat4 viewProjection = projection * glm::mat4(glm::mat3(view));
    skybox_shader->use();
    skybox_shader->apply_uniform_mat4(viewProjection, "viewProjection");

    sky->render(&target_camera, skybox_shader, 0);
  }
  glm::mat4 viewProjection = projection * view;
  for (auto collection : objects) {
    const shader *current_shader = collection.first;
    current_shader->use();
    current_shader->apply_uniform_mat4(viewProjection, "viewProjection");
    current_shader->apply_uniform_vec3(target_camera.get_position(), "viewPos");
    current_shader->apply_uniform_vec3(ambient_light, "ambientLight");

    uint32_t i = 0;
    for (auto light : lights) {
      std::string name = "lights[" + std::to_string(i) + "]";
      current_shader->apply_uniform_vec3(light->get_position(),
                                         name + ".position");
      current_shader->apply_uniform_vec3(light->get_color(), name + ".color");
      current_shader->apply_uniform_mat4(light->get_light_space(),
                                         name + ".lightSpaceMatrix");
      current_shader->apply_uniform_scalar(light->get_range(), name + ".range");
      light->use_depth_map(i);
      current_shader->apply_uniform(i, name + ".depthMap");
      i++;
    }
    current_shader->apply_uniform(lights.size(), "numLights");

    for (const object *obj : collection.second) {
      obj->render(&target_camera, current_shader, i);
    }
    glUseProgram(0);
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
    for (auto collection : objects) {
      for (const object *obj : collection.second) {
        light_pass_shader->apply_uniform_mat4(obj->get_model_matrix(), "model");
        obj->draw();
      }
    }
    // cleanup
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  glUseProgram(0);
  glCullFace(GL_BACK);
}

void scene::main(camera *target_camera, bool *should_close) {
  while (!*should_close) {
    if (!this->initialized) {
      continue;
    }
    {
      double new_time = glfwGetTime();
      delta_time = new_time - current_time;
      current_time = new_time;
    }
    this->update(target_camera, delta_time, current_time);
  }
}

void scene::update(camera *, double, double) {}

void scene::scroll_callback(double, double, camera &) {
  // do nothing
}

void scene::key_callback(int, int, int, int, camera &) {
  // do nothing
}

void scene::mouse_button_callback(int, int, int, camera &) {
  // do nothing
}

void scene::mouse_callback(double, double, camera &) {
  // do nothing
}

bool scene::check_point(glm::vec3 point) const {
  for (const collider *obj : colliders) {
    if (obj->check_point(point)) {
      return true;
    }
  }
  return false;
}

bool scene::check_line(glm::vec3 a, glm::vec3 b) const {
  for (const collider *obj : colliders) {
    if (obj->check_line(a, b)) {
      return true;
    }
  }
  return false;
}
