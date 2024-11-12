
#pragma once

#include "include.hpp"

#include <vector>
#include <map>

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
        float scale;
        double xrot, yrot, zrot;
        const shader* object_shader;
        unsigned int vertex_count;
        unsigned int index_count;
        // map name->texture
        std::map<std::string, texture*> textures;
        unsigned int texture_count;
        unsigned int normal_count;
        /*!
         @brief the data of the object (vertices, normals, texture coordinates, tangents and bitangents)
        */
        std::vector<float> data;
        /*!
         @brief the indices of the object
        */
        std::vector<unsigned int> indices;
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
         @brief Initialize the object
         @details Initializes the object by creating the VAO, VBO and EBO
         @warning This function must be called before rendering the object, and should only be called once per context
        */
        void init();
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
        void add_texture(texture* tex, std::string name);
        /*!
         @brief Set the position of the object
         @param xpos The x position of the object
         @param ypos The y position of the object
         @param zpos The z position of the object
        */
        void set_position(double xpos, double ypos, double zpos);
        /*!
         @brief Set the scale of the object
         @param scale The scale of the object
        */
        void set_scale(float scale);
        /*!
         @brief Set the rotation of the object
         @param xrot The x rotation of the object
         @param yrot The y rotation of the object
         @param zrot The z rotation of the object
        */
        void set_rotation(double xrot, double yrot, double zrot);
};
