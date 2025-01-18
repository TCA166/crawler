
#pragma once

#include <map>
#include <stdint.h>
#include <string>

typedef struct {
  const uint8_t *data;
  uint32_t width, height;
  uint8_t nr_channels;
} image_t;

class image_loader {
private:
  image_loader();
  std::map<std::string, image_t *> images;

public:
  static image_loader &get();
  const image_t *load_image(const std::string &key, bool flip);
  ~image_loader();
};
