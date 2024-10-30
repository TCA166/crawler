
#pragma once

#include <glm/glm.hpp>

class camera {
    private:
        glm::vec3 position;
        glm::vec3 front;
        float speed;
        float fov;
    public:
        camera(glm::vec3 position, glm::vec3 front, float speed, float fov);
        camera();
        ~camera();
        void move(glm::vec3 direction);
        void zoom(float value);
        void update_front(double xoffset, double yoffset);
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix(float aspect_ratio);
};
