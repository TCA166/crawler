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
    /*!
     @brief Set the active texture
     @param target_shader The shader to set the texture in
     @param texture_unit The texture unit to set the texture to
    */
    virtual void set_active_texture(const shader *target_shader,
                                    int texture_unit, std::string name) const;
};
