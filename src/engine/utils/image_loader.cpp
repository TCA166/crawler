#include "image_loader.hpp"

#include "../settings.hpp"

#include <stdexcept>

#ifndef STATIC_ASSETS
#include <SOIL/SOIL.h>
#else
#include "../../../static/textures/spaceship.h"
#include "../../../static/textures/spaceship_normal.h"
#include "../../../static/textures/texture.h"
#endif

void flip_y(unsigned char *img, int width, int height, int channels) {
  int i, j;
  for (j = 0; j * 2 < height; ++j) {
    int index1 = j * width * channels;
    int index2 = (height - 1 - j) * width * channels;
    for (i = width * channels; i > 0; --i) {
      unsigned char temp = img[index1];
      img[index1] = img[index2];
      img[index2] = temp;
      ++index1;
      ++index2;
    }
  }
}

image_loader::image_loader() {
#ifdef STATIC_ASSETS
  image_t *spaceship_image = new image_t;
  spaceship_image->data = textures_spaceship_jpg;
  spaceship_image->width = 4096;
  spaceship_image->height = 4096;
  spaceship_image->nr_channels = 3;
  images[TEXTURE_PATH("spaceship.jpg")] = spaceship_image;
  image_t *spaceship_normal_image = new image_t;
  spaceship_normal_image->data = textures_spaceship_normal_jpg;
  spaceship_normal_image->width = 4096;
  spaceship_normal_image->height = 4096;
  spaceship_normal_image->nr_channels = 3;
  images[TEXTURE_PATH("spaceship_normal.jpg")] = spaceship_normal_image;
  image_t *texture_image = new image_t;
  texture_image->data = textures_texture_png;
  texture_image->width = 996;
  texture_image->height = 664;
  texture_image->nr_channels = 3;
  images[TEXTURE_PATH("texture.png")] = texture_image;
  images[TEXTURE_PATH("skybox/right.png")] = spaceship_image;
  images[TEXTURE_PATH("skybox/left.png")] = spaceship_image;
  images[TEXTURE_PATH("skybox/top.png")] = spaceship_image;
  images[TEXTURE_PATH("skybox/bottom.png")] = spaceship_image;
  images[TEXTURE_PATH("skybox/front.png")] = spaceship_image;
  images[TEXTURE_PATH("skybox/back.png")] = spaceship_image;
#endif
}

image_loader &image_loader::get() {
  static image_loader instance;
  return instance;
}

const image_t *image_loader::load_image(const std::string &key, bool flip) {
  try {
    return images.at(key);
  } catch (const std::out_of_range &e) {
#ifndef STATIC_ASSETS
    int width, height, nr_channels;
    unsigned char *image = SOIL_load_image(key.c_str(), &width, &height,
                                           &nr_channels, SOIL_LOAD_AUTO);
    if (image == nullptr) {
      throw std::runtime_error(SOIL_last_result());
    }
    if (flip) {
      flip_y(image, width, height, nr_channels);
    }
    image_t *new_image = new image_t;
    new_image->data = image;
    new_image->width = width;
    new_image->height = height;
    new_image->nr_channels = nr_channels;
    images[key] = new_image;
    return new_image;
#else
    throw std::runtime_error("Image not found");
#endif
  }
}

image_loader::~image_loader() {
  for (auto &pair : images) {
#ifndef STATIC_ASSETS
    SOIL_free_image_data((unsigned char *)pair.second->data);
#endif
    delete pair.second;
  }
}
