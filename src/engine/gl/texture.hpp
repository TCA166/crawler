
#pragma once

#include <string>

#include "shader.hpp"

/*!
 @brief Texture class to handle textures.
 @details This class is used to load and bind textures.
 */
class texture {
  protected:
    /*!
     @brief The id of the texture
    */
    GLuint texture_id;

  public:
    /*!
     @brief Initializes the object assuming a texture has already been loaded
     @param texture_id The id of the texture
     @note Meant to be used in inherited classes
    */
    texture(GLuint texture_id);
    /*!
     @brief Constructs a texture based on a path
     @param path Path to the texture
     @param flip Whether to flip the texture
    */
    texture(const std::string &path, bool flip);
    /*!
     @brief Constructs a texture based on a path
     @param path Path to the texture
     @note flips the texture
    */
    texture(const std::string &path);
    virtual ~texture();
    /*!
     @brief Set the active texture
     @param target_shader The shader to set the texture in
     @param texture_unit The texture unit to set the texture to
     @param name The name of the texture
    */
    virtual void set_active_texture(const shader *target_shader,
                                    int texture_unit, std::string name) const;
};

/*!
 @brief Flip an image vertically
 @param img The image to flip
 @param width The width of the image
 @param height The height of the image
 @param channels The number of channels in the image
*/
void flip_y(unsigned char *img, int width, int height, int channels);
