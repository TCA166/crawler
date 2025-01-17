#include "image_loader.hpp"

#include "../settings.hpp"

#include <stdexcept>

#ifndef STATIC_ASSETS
#include <SOIL/SOIL.h>
#else
#include "../../../textures/spaceship.c"
#include "../../../textures/spaceship_normal.c"
#include "../../../textures/texture.c"
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
  spaceship_image->data = spaceship.pixel_data;
  spaceship_image->width = spaceship.width;
  spaceship_image->height = spaceship.height;
  spaceship_image->nr_channels = spaceship.bytes_per_pixel;
  images[TEXTURE_PATH("spaceship.jpg")] = spaceship_image;
  image_t *spaceship_normal_image = new image_t;
  spaceship_normal_image->data = spaceship_normal.pixel_data;
  spaceship_normal_image->width = spaceship_normal.width;
  spaceship_normal_image->height = spaceship_normal.height;
  spaceship_normal_image->nr_channels = spaceship_normal.bytes_per_pixel;
  images[TEXTURE_PATH("spaceship_normal.jpg")] = spaceship_normal_image;
  image_t *texture_image = new image_t;
  texture_image->data = texture.pixel_data;
  texture_image->width = texture.width;
  texture_image->height = texture.height;
  texture_image->nr_channels = texture.bytes_per_pixel;
  images[TEXTURE_PATH("texture.jpg")] = texture_image;
  /* TEXTURE_PATH("skybox/right.png"), TEXTURE_PATH("skybox/left.png"),
  TEXTURE_PATH("skybox/top.png"), TEXTURE_PATH("skybox/bottom.png"),
      TEXTURE_PATH("skybox/front.png"), TEXTURE_PATH("skybox/back.png") */
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
