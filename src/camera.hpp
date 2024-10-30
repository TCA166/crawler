
#pragma once

#include <glm/glm.hpp>

class camera {
    private:
        glm::vec3 position;
        glm::vec3 front;
        float speed;
        float look_speed;
        float fov;
        float pitch;
        float yaw;
    public:
        camera(glm::vec3 position, float speed, float fov, float look_speed, float pitch, float yaw);
        camera();
        ~camera();
        void move(glm::vec3 direction);
        void zoom(float value);
        void move_front(double xoffset, double yoffset);
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix(float aspect_ratio);
};
