
#include "object.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

#include "../utils/collision.hpp"
#include "../utils/model_loader.hpp"

object::object(const model *object_model, double xpos, double ypos, double zpos)
    : scale(glm::vec3(1.)), rot(glm::vec3(0.)), object_model(object_model),
      position(xpos, ypos, zpos) {}

object::~object() {
  for (object *parent : parents) {
    parent->remove_child(this);
  }
}

glm::mat4 object::get_model_matrix() const {
  return glm::translate(glm::mat4(1.0f), position) *
         glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
         glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
         glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
         glm::scale(glm::mat4(1.0f), scale);
}

void object::render(const camera *, const shader *current_shader,
                    uint32_t tex_off) const {

  size_t tex_i = tex_off;
  for (const auto &pair : textures) {
    pair.second->set_active_texture(current_shader, tex_i, pair.first);
    tex_i++;
  }

  current_shader->apply_uniform_mat4(get_model_matrix(), "model");

  this->draw();

  // unbind textures
  for (size_t i = tex_off; i < tex_i; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void object::draw() const { object_model->draw(); }

void object::add_texture(const texture *tex, std::string name) {
  textures[name] = tex;
}

void object::set_position(glm::vec3 position) {
  this->position = position;
  for (moveable *child : children) {
    child->set_position(position);
  }
}

void object::set_scale(float scalex, float scaley, float scalez) {
  this->scale = glm::vec3(scalex, scaley, scalez);
}

void object::set_scale(float scale) { this->scale = glm::vec3(scale); }

void object::rotate(glm::vec3 rotation) {
  this->rot += rotation;
  for (moveable *child : children) {
    child->rotate(rotation);
  }
}

void object::set_rotation(glm::vec3 rotation) {
  this->rot = rotation;
  for (moveable *child : children) {
    child->set_rotation(rotation);
  }
}

void object::translate(glm::vec3 translation) {
  this->position += translation;
  for (moveable *child : children) {
    child->translate(translation);
  }
}

glm::vec3 object::get_position() const { return position; }

void object::add_child(moveable *child) { children.push_back(child); }

void object::remove_child(const moveable *child) {
  children.remove(const_cast<moveable *>(child));
}

void object::add_parent(object *parent) {
  this->parents.push_back(parent);
  parent->add_child(this);
}

glm::vec3 object::get_bounds() const {
  glm::mat4 model = get_model_matrix();
  glm::vec4 bounds = model * glm::vec4(this->object_model->get_bounds(), 1.0f);
  return bounds;
}

glm::vec3 object::get_negbounds() const {
  glm::mat4 model = get_model_matrix();
  glm::vec4 negbounds =
      model * glm::vec4(this->object_model->get_negbounds(), 1.0f);
  return negbounds;
}

bool object::check_point(glm::vec3 point) const {
  glm::vec3 bounds = get_bounds();
  glm::vec3 negbounds = get_negbounds();
  return point.x <= bounds.x && point.x >= negbounds.x && point.y <= bounds.y &&
         point.y >= negbounds.y && point.z <= bounds.z &&
         point.z >= negbounds.z;
}

bool object::check_line(glm::vec3 a, glm::vec3 b) const {
  return check_line_box(get_negbounds(), get_bounds(), a, b, a);
}