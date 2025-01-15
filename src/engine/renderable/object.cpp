
#include "object.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

#include "../utils/collision.hpp"
#include "../utils/model_loader.hpp"

object::object(const shader *object_shader, const model *object_model,
               double xpos, double ypos, double zpos)
    : object_shader(object_shader), scale(glm::vec3(1.)), rot(glm::vec3(0.)),
      object_model(object_model), xpos(xpos), ypos(ypos), zpos(zpos) {}

object::object(const shader *object_shader, const std::string &path,
               double xpos, double ypos, double zpos)
    : object(object_shader, model_loader::get().get_model(path), xpos, ypos,
             zpos) {}

object::~object() {
  for (object *parent : parents) {
    parent->remove_child(this);
  }
}

glm::mat4 object::get_model_matrix() const {
  return glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos)) *
         glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
         glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
         glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
         glm::scale(glm::mat4(1.0f), scale);
}

void object::render(const glm::mat4 *viewProjection, glm::vec3 viewPos,
                    const std::list<const light *> &lights,
                    glm::vec3 ambient) const {
  object_shader->use();

  size_t tex_i = 0;
  for (const auto &pair : textures) {
    pair.second->set_active_texture(object_shader, tex_i, pair.first);
    tex_i++;
  }

  object_shader->apply_uniform_mat4(this->get_model_matrix(), "model");
  object_shader->apply_uniform_mat4(*viewProjection, "viewProjection");
  object_shader->apply_uniform_vec3(ambient, "ambientLight");
  object_shader->apply_uniform_vec3(viewPos, "viewPos");
  // TODO do something about shininess
  object_shader->apply_uniform_scalar(1e10, "shininess");
  object_shader->apply_uniform_scalar(glfwGetTime(), "time");

  size_t i = 0;
  // Pass light properties to the shader
  for (auto light : lights) {
    std::string name = "lights[" + std::to_string(i) + "]";
    object_shader->apply_uniform_vec3(light->get_position(),
                                      name + ".position");
    object_shader->apply_uniform_vec3(light->get_color(), name + ".color");
    object_shader->apply_uniform_mat4(light->get_light_space(),
                                      name + ".lightSpaceMatrix");
    light->use_depth_map(tex_i);
    object_shader->apply_uniform(tex_i, name + ".depthMap");
    tex_i++;
    i++;
  }
  object_shader->apply_uniform(lights.size(), "numLights");
  this->draw();
  glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
  glUseProgram(0);
}

void object::draw() const { object_model->draw(); }

void object::render(const camera *target_camera, float aspect_ratio,
                    const std::list<const light *> &lights,
                    glm::vec3 ambient) const {
  glm::mat4 viewProjection =
      target_camera->get_projection_matrix(aspect_ratio) *
      target_camera->get_view_matrix();
  this->render(&viewProjection, target_camera->get_position(), lights, ambient);
}

void object::add_texture(const texture *tex, std::string name) {
  textures[name] = tex;
}

void object::set_position(double xpos, double ypos, double zpos) {
  this->xpos = xpos;
  this->ypos = ypos;
  this->zpos = zpos;
  for (moveable *child : children) {
    child->set_position(xpos, ypos, zpos);
  }
}

void object::set_scale(float scalex, float scaley, float scalez) {
  this->scale = glm::vec3(scalex, scaley, scalez);
}

void object::set_scale(float scale) { this->scale = glm::vec3(scale); }

void object::rotate(double xrot, double yrot, double zrot) {
  this->rot.x += xrot;
  this->rot.y += yrot;
  this->rot.z += zrot;
  for (moveable *child : children) {
    child->rotate(xrot, yrot, zrot);
  }
}

void object::set_rotation(double xrot, double yrot, double zrot) {
  double xdiff = xrot - this->rot.x;
  double ydiff = yrot - this->rot.y;
  double zdiff = zrot - this->rot.z;
  this->rotate(xdiff, ydiff, zdiff);
}

void object::translate(glm::vec3 translation) {
  xpos += translation.x;
  ypos += translation.y;
  zpos += translation.z;
  for (moveable *child : children) {
    child->translate(translation);
  }
}

glm::vec3 object::get_position() const { return glm::vec3(xpos, ypos, zpos); }

void object::add_child(moveable *child) { children.push_back(child); }

void object::remove_child(const moveable *child) {
  children.remove(const_cast<moveable *>(child));
}

void object::add_parent(object *parent) {
  this->parents.push_back(parent);
  parent->add_child(this);
}

bool object::check_point(glm::vec3 point) const {
  glm::mat4 model = get_model_matrix();
  glm::vec4 bounds = model * glm::vec4(this->object_model->get_bounds(), 1.0f);
  glm::vec4 negbounds =
      model * glm::vec4(this->object_model->get_negbounds(), 1.0f);
  return point.x <= bounds.x && point.x >= negbounds.x && point.y <= bounds.y &&
         point.y >= negbounds.y && point.z <= bounds.z &&
         point.z >= negbounds.z;
}

bool object::check_line(glm::vec3 a, glm::vec3 b) const {
  glm::mat4 model = get_model_matrix();
  glm::vec4 bounds = model * glm::vec4(this->object_model->get_bounds(), 1.0f);
  glm::vec4 negbounds =
      model * glm::vec4(this->object_model->get_negbounds(), 1.0f);
  return check_line_box(glm::vec3(negbounds), glm::vec3(bounds), a, b, a);
}
