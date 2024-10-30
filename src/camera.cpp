#include "camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define DEFAULT_DIR glm::vec3(1.f, 0.f, 0.f)
#define DEFAULT_POS glm::vec3(0.0f, 0.0f, 0.0f)
#define DEFAULT_SPEED 0.1f
#define DEFAULT_FOV 90.0f

camera::camera(glm::vec3 position, glm::vec3 front, float speed, float fov) {
    this->position = position;
    this->front = front;
    this->speed = speed;
    this->fov = fov;
}

camera::camera() : camera(DEFAULT_POS, DEFAULT_DIR, DEFAULT_SPEED, DEFAULT_FOV) {

}

camera::~camera() {

}

void camera::move(glm::vec3 direction) {
    position += direction * speed;
}

void camera::zoom(float value) {
    fov += value;
}

void camera::update_front(double xoffset, double yoffset) {
    front.x = cos(glm::radians(xoffset)) * cos(glm::radians(yoffset));
    front.y = sin(glm::radians(yoffset));
    front.z = sin(glm::radians(xoffset)) * cos(glm::radians(yoffset));
    front = glm::normalize(front);
}

glm::mat4 camera::get_view_matrix() {
    return glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) {
    return glm::perspective(glm::radians(fov), aspect_ratio, 0.1f, 100.0f);
}
