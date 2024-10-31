
#pragma once

#include <GL/glew.h>
#include <string>

#include "shader.hpp"

/*!
 @brief Texture class to handle textures.
 @details This class is used to load and bind textures.
 */
class texture {
    private:
        GLuint texture_id;
        int width, height, nr_channels;
        std::string name;
    public:
        /*!
         @brief Constructs a texture based on a path
         @param path Path to the texture
         @param name The name of the texture in the shader program
        */
        texture(const std::string& path, std::string name);
        ~texture();
        /*!
         @brief Set the active texture
         @param target_shader The shader to set the texture in
         @param texture_unit The texture unit to set the texture to
        */
        void set_active_texture(const shader* target_shader, int texture_unit) const;
};
