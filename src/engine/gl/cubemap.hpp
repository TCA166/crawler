#pragma once

#include <vector>

#include "texture.hpp"

/*!
 @brief A cubemap texture
*/
class cubemap : public texture {
  public:
    /*!
     @brief Create a new cubemap texture
     @param paths The paths to the cubemap textures
    */
    cubemap(const std::vector<std::string> &paths);
    ~cubemap();
};
