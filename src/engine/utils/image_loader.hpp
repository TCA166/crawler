
#pragma once

#include <map>
#include <stdint.h>
#include <string>

/*!
 @brief A simple struct for storing image data
*/
typedef struct {
  /*!
   @brief The data of the image in the format indicated by nr_channels
  */
  uint8_t *data;
  /*!
   @brief The width of the image in pixels
  */
  uint32_t width;
  /*!
   @brief The height of the image in pixels
  */
  uint32_t height;
  /*!
   @brief The number of bytes per pixel
  */
  uint8_t nr_channels;
} image_t;

/*!
 @brief A facility for loading images
 @details It caches loaded images, and also allows us to have some assets
 preloaded at compile time
*/
class image_loader {
private:
  image_loader();
  std::map<std::string, image_t *> images;

public:
  /*!
   @brief Gets the instance of the singleton
   @return image_loader instance
  */
  static image_loader &get();
  /*!
   @brief Loads or retrieves an image
   @param key the path to the image
   @param flip whether the image should be flipped when loaded
   @return The requested image
  */
  const image_t *load_image(const std::string &key, bool flip);
  ~image_loader();
};
