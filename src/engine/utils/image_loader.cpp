#include "image_loader.hpp"

#include <stdexcept>

#ifndef WEBASM
#include <SOIL/SOIL.h>
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

image_loader::image_loader() {}

image_loader &image_loader::get() {
  static image_loader instance;
  return instance;
}

const image_t *image_loader::load_image(const std::string &key, bool flip) {
  try {
    return images.at(key);
  } catch (const std::out_of_range &e) {
#ifndef WEBASM
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
#ifndef WEBASM
    SOIL_free_image_data(pair.second->data);
#endif
    delete pair.second;
  }
}
