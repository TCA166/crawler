#pragma once

#include "../engine/engine.hpp"

/*!
 @brief A cube with a texture and a normal map
*/
class debug_cube : public cube {
private:
  const texture tex, norm;

public:
  /*!
   @brief Constructs a debug cube object
   @param object_shader The shader to use for rendering
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  debug_cube(const shader *object_shader, double xpos, double ypos,
             double zpos);
  ~debug_cube();
};

inline debug_cube::debug_cube(const shader *object_shader, double xpos,
                              double ypos, double zpos)
    : cube(object_shader, xpos, ypos, zpos), tex(TEXTURE_PATH("spaceship.jpg")),
      norm(TEXTURE_PATH("spaceship_normal.jpg")) {
  this->add_texture(&tex, "texture0");
  this->add_texture(&norm, "normal0");
}

inline debug_cube::~debug_cube() {}
