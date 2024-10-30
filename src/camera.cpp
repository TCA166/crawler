#include "camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

camera::camera(glm::vec3 position, float speed, float fov, float look_speed, float pitch, float yaw) {
    this->position = position;
    this->speed = speed;
    this->fov = fov;
    this->look_speed = look_speed;
    this->pitch = pitch;
    this->yaw = yaw;
    this->move_front(0.0, 0.0);
}

camera::camera() : camera(glm::vec3(1.0f, 1.0f, 0.0f), 0.1f, 90.0f, 0.1f, 0.0f, 0.0f) {

}

camera::~camera() {

}

void camera::move(glm::vec3 direction) {
    position += direction * speed;
}

void camera::zoom(float value) {
    fov += value;
}

void camera::move_front(double xoffset, double yoffset) {
    pitch += yoffset * look_speed;
    yaw += xoffset * look_speed;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);
}

glm::mat4 camera::get_view_matrix() {
    return glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) {
    return glm::perspective(glm::radians(fov), aspect_ratio, NEAR_PLANE, FAR_PLANE);
}
