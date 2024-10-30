
#pragma once

#include <GL/glew.h>

#include "shader.hpp"
#include "camera.hpp"

/*!
 @brief Object class to handle rendering of objects.
*/
class object {
    private:
        GLuint VAO, VBO;
        double xpos, ypos;
        const shader* object_shader;
    public:
        /*!
         @brief Constructs an object with a given shader and vertices
         @param object_shader The shader to use for rendering
         @param vertices_colors The vertices and colors of the object
         @param size The size of the vertices_colors array
        */
        object(const shader* object_shader, const float* vertices_colors, size_t size);
        /*!
         @brief Constructs an object with a given shader, vertices and position
         @param object_shader The shader to use for rendering
         @param vertices_colors The vertices and colors of the object, interleaved as [x, y, z, w, r, g, b, a]
         @param size The size of the vertices_colors array
         @param xpos The x position of the object
         @param ypos The y position of the object
        */
        object(const shader* object_shader, const float* vertices_colors, size_t size, double xpos, double ypos);
        ~object();
        /*!
         @brief Render the object
         @param target_camera The camera to render the object with
         @param aspect_ratio The aspect ratio of the window
        */
        void render(const camera* target_camera, float aspect_ratio);
};
