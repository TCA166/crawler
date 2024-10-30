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
    this->update_front();
}

camera::camera() : camera(glm::vec3(1.0f, 1.0f, 0.0f), 10.0f, 90.0f, 0.1f, 0.0f, 0.0f) {

}

camera::~camera() {

}

void camera::move_up(float scale) {
    position.y += scale * speed;
}

void camera::move_forward(float scale) {
    position += front * scale * speed;
}

void camera::move_right(float scale) {
    position += glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * scale * speed;
}

void camera::zoom(float value) {
    float new_fov = fov + value;
    if (new_fov >= 180.0f || new_fov <= 1.0f){
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

void camera::move_front(double xoffset, double yoffset) {
    pitch += yoffset * look_speed;
    yaw += xoffset * look_speed;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    update_front();
}

glm::mat4 camera::get_view_matrix() {
    return glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) {
    return glm::perspective(glm::radians(fov), aspect_ratio, NEAR_PLANE, FAR_PLANE);
}
