#include "camera.hpp"

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100000.0f

#define MAX_TURN 89.0f

#define UP glm::vec3(0.0f, 1.0f, 0.0f)

camera::camera(glm::vec3 position, float speed, float fov, float look_speed,
               float pitch, float yaw)
    : position(position), speed(speed), look_speed(look_speed), fov(fov),
      pitch(pitch), yaw(yaw) {
    this->update_front();
}

camera::camera(glm::vec3 position)
    : camera(position, 1.0f, 90.0f, 0.1f, 0.0f, 0.0f) {}

camera::~camera() {}

void camera::move_up(float scale) { this->translate(UP * scale * speed); }

void camera::move_forward(float scale) {
    this->translate(front * scale * speed);
}

void camera::move_right(float scale) {
    this->translate(
        glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * scale *
        speed);
}

void camera::zoom(float value) {
    float new_fov = fov + value;
    if (new_fov >= 180.0f || new_fov <= 1.0f) {
        return;
    }
    fov = new_fov;
}

void camera::update_front() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}

void camera::rotate_front(double xoffset, double yoffset) {
    pitch += yoffset * look_speed;
    yaw += xoffset * look_speed;

    if (pitch > MAX_TURN) {
        pitch = MAX_TURN;
    }
    if (pitch < -MAX_TURN) {
        pitch = -MAX_TURN;
    }

    update_front();
}

glm::mat4 camera::get_view_matrix() const {
    return glm::lookAt(position, position + front, UP);
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
    return glm::perspective(glm::radians(fov), aspect_ratio, NEAR_PLANE,
                            FAR_PLANE);
}

glm::vec3 camera::get_position() const { return position; }

void camera::set_position(double x, double y, double z) {
    this->position = glm::vec3(x, y, z);
}

void camera::translate(glm::vec3 translation) { position += translation; }

void camera::set_speed(float speed) { this->speed = speed; }

void camera::rotate(double xrot, double yrot, double) {
    this->rotate_front(xrot, yrot);
}

void camera::set_rotation(double xrot, double yrot, double) {
    this->pitch = xrot;
    this->yaw = yrot;
    this->update_front();
}
