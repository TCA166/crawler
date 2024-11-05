
#pragma once

#include <GL/glew.h>

#include <vector>

#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "light.hpp"

/*!
 @brief Object class to handle rendering of objects.
*/
class object {
    protected:
        GLuint VAO, VBO, EBO;
        double xpos, ypos, zpos;
        const shader* object_shader;
        unsigned int vertex_count;
        unsigned int index_count;
        std::vector<texture*> textures;
        unsigned int texture_count;
        unsigned int normal_count;
    public:
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
         @param lights The lights to render the object with
        */
        void render(const camera* target_camera, float aspect_ratio, const std::vector<const light*>& lights, glm::vec3 ambient) const;
        /*!
         @brief Render the object
         @param viewProjection The view projection matrix to render the object with
         @param lights The lights to render the object with
        */
        virtual void render(const glm::mat4* viewProjection, glm::vec3 viewPos, const std::vector<const light*>& lights, glm::vec3 ambient) const;
        /*!
         @brief Add a texture to the object
         @param tex The texture to add
        */
        void add_texture(texture* tex);
};
