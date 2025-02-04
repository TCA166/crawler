
#include "texture.hpp"

#include <stdexcept>

texture::texture(GLuint texture_id) : texture_id(texture_id) {}

// by default flip the image, this is because SOIL loads the image upside down
texture::texture(const std::string &path) : texture(path, true) {}

texture::texture(const std::string &path, bool flip)
    : texture(image_loader::get().load_image(path, flip)) {}

texture::texture(const image_t *img) {
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  int format = GL_RGB;
  if (img->nr_channels == 4) {
    format = GL_RGBA;
  }
  glTexImage2D(GL_TEXTURE_2D, 0, format, img->width, img->height, 0, format,
               GL_UNSIGNED_BYTE, img->data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

texture::~texture() { glDeleteTextures(1, &texture_id); } // FIXME segfault
                                                          // here?

void texture::set_active_texture(const shader *target_shader, int texture_unit,
                                 std::string name) const {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  target_shader->apply_uniform(texture_unit, name);
}

void texture::bind_to_fb() const {
  glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D, texture_id, 0);
}
