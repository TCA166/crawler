#include "camera.hpp"

#include "../settings.hpp"

#define MAX_TURN glm::radians(89.0f)

#define UP glm::vec3(0.0f, 1.0f, 0.0f)

camera::camera(glm::vec3 position, float fov, float look_speed, float pitch,
               float yaw, float roll)
    : position(position), look_speed(look_speed), fov(fov), yaw(yaw),
      pitch(pitch), roll(roll) {
  this->update_front();
}

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

void camera::update_front() {
  front.x = cos(yaw) * cos(pitch);
  front.y = sin(pitch);
  front.z = sin(yaw) * cos(pitch);
  front = glm::normalize(front);
}

glm::mat4 camera::get_view_matrix() const {
  return glm::lookAt(position, position + front, this->get_up());
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
  return glm::perspective(glm::radians(fov), aspect_ratio, RENDER_MIN,
                          RENDER_MAX);
}

glm::vec3 camera::get_position() const { return position; }

void camera::set_position(double x, double y, double z) {
  this->position = glm::vec3(x, y, z);
}

void camera::translate(glm::vec3 translation) { position += translation; }

void camera::rotate(double xrot, double yrot, double zrot) {
  pitch += yrot * look_speed;
  yaw += xrot * look_speed;

  if (pitch > MAX_TURN) {
    pitch = MAX_TURN;
  }
  if (pitch < -MAX_TURN) {
    pitch = -MAX_TURN;
  }

  update_front();
  roll += zrot;
}

void camera::set_rotation(double xrot, double yrot, double zrot) {
  this->yaw = xrot;
  this->pitch = yrot;
  this->update_front();
  roll = zrot;
}

glm::vec3 camera::get_front() const { return front; }

glm::vec3 camera::get_up() const {
  return glm::rotate(glm::mat4(1), roll, front) * glm::vec4(UP, 1.0f);
}

glm::vec3 camera::get_right() const {
  return glm::normalize(glm::cross(front, UP));
}

glm::vec3 camera::get_rotation() const { return glm::vec3(pitch, yaw, roll); }
