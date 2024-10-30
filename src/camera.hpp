
#pragma once

#include <glm/glm.hpp>

/*!
 @brief Camera class to handle camera movement.
*/
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
        /*!
         @brief Constructs a camera with a given position, speed, fov, look speed, pitch and yaw.
         @param position The position of the camera
         @param speed The speed of the camera
         @param fov The field of view of the camera
         @param look_speed The look speed of the camera
         @param pitch The pitch of the camera
         @param yaw The yaw of the camera
        */
        camera(glm::vec3 position, float speed, float fov, float look_speed, float pitch, float yaw);
        /*!
         @brief Constructs a camera with default values.
        */
        camera();
        ~camera();
        /*!
         @brief Move the camera in a given direction.
         @param direction The direction to move the camera
        */
        void move(glm::vec3 direction);
        /*!
         @brief Zooms in the camera by adjusting the fov
         @param value The value to zoom in by
        */
        void zoom(float value);
        /*!
         @brief Turns the camera by adjusting the pitch and yaw values
         @details The provided x and y offsets are used to adjust the pitch and yaw values by turning them into radians.
         @param xoffset The x offset to turn the camera
         @param yoffset The y offset to turn the camera
        */
        void move_front(double xoffset, double yoffset);
        /*!
         @brief Get the view matrix of the camera
         @return The view matrix of the camera
        */
        glm::mat4 get_view_matrix();
        /*!
         @brief Get the projection matrix of the camera
         @param aspect_ratio The aspect ratio of the window
         @return The projection matrix of the camera
        */
        glm::mat4 get_projection_matrix(float aspect_ratio);
};
