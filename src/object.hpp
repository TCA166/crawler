
#pragma once

#include <GL/glew.h>

#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"

/*!
 @brief Object class to handle rendering of objects.
*/
class object {
    private:
        GLuint VAO, VBO, EBO;
        double xpos, ypos, zpos;
        const shader* object_shader;
        std::vector<texture*> textures;
        unsigned int vertex_count;
        unsigned int index_count;
        unsigned int texture_count;
        unsigned int normal_count;
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
        object(const shader* object_shader, const float* vertices_colors, size_t size, double xpos, double ypos, double zpos);
        /*!
         @brief Constructs an object with a given shader and path to an obj file
         @param object_shader The shader to use for rendering
         @param path The path to the obj file
         @param xpos The x position of the object
         @param ypos The y position of the object
         @param zpos The z position of the object
        */
        object(const shader* object_shader, const std::string& path, double xpos, double ypos, double zpos);
        ~object();
        /*!
         @brief Render the object
         @param target_camera The camera to render the object with
         @param aspect_ratio The aspect ratio of the window
        */
        void render(const camera* target_camera, float aspect_ratio) const;
        /*!
         @brief Add a texture to the object
         @param tex The texture to add
        */
        void add_texture(texture* tex);
};
