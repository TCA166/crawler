#include "cubemap.hpp"

#include "texture.hpp"

#include <SOIL/SOIL.h>

#include <stdexcept>

/*!
 @brief helper function to actually do what a cubemap constructor should do
 because C++ is DUMB
 @param paths The paths to the cubemap textures
*/
static GLuint load_cubemap(const std::vector<std::string> &paths) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    for (unsigned int i = 0; i < paths.size(); i++) {
        int width, height, nr_channels;
        unsigned char *image = SOIL_load_image(
            paths[i].c_str(), &width, &height, &nr_channels, SOIL_LOAD_AUTO);
        if (image == nullptr) {
            throw std::runtime_error(SOIL_last_result());
        }
        flip_y(image, width, height, nr_channels);
        int format = GL_RGB;
        if (nr_channels == 4) {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width,
                     height, 0, format, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture_id;
}
cubemap::cubemap(const std::vector<std::string> &paths)
    : texture(load_cubemap(paths)) {}

cubemap::~cubemap() {}
