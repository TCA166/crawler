#pragma once

#include "../engine/engine.hpp"

class debug_cube : public cube {
  public:
    debug_cube(const shader *object_shader, double xpos, double ypos,
               double zpos);
    ~debug_cube();

  private:
    texture *tex, *norm;
};

inline debug_cube::debug_cube(const shader *object_shader, double xpos,
                              double ypos, double zpos)
    : cube(object_shader, xpos, ypos, zpos) {
    tex = new texture(TEXTURE_PATH("spaceship.jpg"));
    norm = new texture(TEXTURE_PATH("spaceship_normal.jpg"));
    this->add_texture(tex, "texture0");
    this->add_texture(norm, "normal0");
}

inline debug_cube::~debug_cube() {
    delete tex;
    delete norm;
}
