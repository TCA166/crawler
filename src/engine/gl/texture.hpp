
#pragma once

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
    public:
        /*!
         @brief Constructs a texture based on a path
         @param path Path to the texture
         @param flip Whether to flip the texture
        */
        texture(const std::string& path, bool flip);
        /*!
         @brief Constructs a texture based on a path
         @param path Path to the texture
        */
        texture(const std::string& path);
        ~texture();
        /*!
         @brief Set the active texture
         @param target_shader The shader to set the texture in
         @param texture_unit The texture unit to set the texture to
        */
        void set_active_texture(const shader* target_shader, int texture_unit, std::string name) const;
};
