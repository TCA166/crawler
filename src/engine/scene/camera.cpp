#include "camera.hpp"

#include "../settings.hpp"

#define MAX_TURN glm::radians(89.0f)

camera::camera(glm::vec3 position, float fov, float look_speed, float pitch,
               float yaw, float roll)
    : position(position), look_speed(look_speed), fov(fov),
      rotation(pitch, yaw, roll) {}

camera::camera(glm::vec3 position)
    : camera(position, 90.0f, 0.005f, 0.0f, 0.0f, 0.0f) {}

camera::~camera() {}

void camera::zoom(float value) {
  float new_fov = fov + value;
  if (new_fov >= 180.0f || new_fov <= 1.0f) {
    return;
  }
  fov = new_fov;
}

glm::mat4 camera::get_view_matrix() const {
  return glm::lookAt(position, position + get_front(), this->get_up());
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
  return glm::perspective(glm::radians(fov), aspect_ratio, RENDER_MIN,
                          RENDER_MAX);
}

glm::vec3 camera::get_position() const { return position; }

void camera::set_position(glm::vec3 position) { this->position = position; }

void camera::translate(glm::vec3 translation) { position += translation; }

void camera::rotate(glm::vec3 rotation) {
  this->rotation += rotation * look_speed;

  if (this->rotation.x > MAX_TURN) {
    this->rotation.x = MAX_TURN;
  }
  if (this->rotation.x < -MAX_TURN) {
    this->rotation.x = -MAX_TURN;
  }
}

void camera::set_rotation(glm::vec3 rotation) { this->rotation = rotation; }

glm::vec3 camera::get_front() const {
  return glm::normalize(glm::vec3(cos(rotation.y) * cos(rotation.x),
                                  sin(rotation.x),
                                  sin(rotation.y) * cos(rotation.x)));
}

glm::vec3 camera::get_up() const {
  return glm::rotate(glm::mat4(1), rotation.z, get_front()) *
         glm::vec4(UP, 1.0f);
}

glm::vec3 camera::get_right() const {
  return glm::normalize(glm::cross(get_front(), get_up()));
}

glm::vec3 camera::get_rotation() const { return rotation; }
