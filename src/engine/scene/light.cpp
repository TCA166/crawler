#include "light.hpp"

#include "../settings.hpp"
#include <stdexcept>

light::light(glm::vec3 position, glm::vec3 color, float fov, float range)
    : position(position), rotation(0.f), color(color), fov(fov), range(range) {
  // create the framebuffer
  glGenFramebuffers(1, &depthMapFBO);
  // create the texture to store the depth values
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES, SHADOW_RES, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // attach the texture to the framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depthMap, 0);
#ifndef WASM
  glDrawBuffer(GL_NONE);
#endif
  glReadBuffer(GL_NONE);

#ifndef WASM
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer is not complete!");
  }
#endif

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

light::~light() {
  glDeleteFramebuffers(1, &depthMapFBO);
  glDeleteTextures(1, &depthMap);
}

const glm::vec3 &light::get_color() const { return color; }

const glm::vec3 &light::get_position() const { return position; }

const glm::mat4 light::get_light_space() const {
  return glm::perspective(fov, 1.0f, 1.0f, range) *
         glm::lookAt(position, this->position + this->get_front(), get_up());
}

void light::look_at(glm::vec3 target) {
  glm::vec3 direction = glm::normalize(target - position);
  rotation.x = asin(direction.y);
  rotation.y = atan2(direction.z, direction.x);
}

void light::set_color(glm::vec3 color) { this->color = color; }

void light::set_position(glm::vec3 position) { this->position = position; }

void light::bind_view_map() const {
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void light::use_depth_map(int texture_unit) const {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, depthMap);
}

texture *light::get_view_map() const { return new texture(depthMap); }

void light::translate(glm::vec3 translation) { position += translation; }

glm::vec3 light::get_front() const {
  return glm::normalize(glm::vec3(cos(rotation.y) * cos(rotation.x),
                                  sin(rotation.x),
                                  sin(rotation.y) * cos(rotation.x)));
}

glm::vec3 light::get_up() const {
  return glm::rotate(glm::mat4(1), rotation.z, get_front()) *
         glm::vec4(UP, 1.0f);
}

void light::rotate(glm::vec3 rotation) { this->rotation += rotation; }

void light::set_rotation(glm::vec3 rotation) { this->rotation = rotation; }

float light::get_range() const { return range; }
