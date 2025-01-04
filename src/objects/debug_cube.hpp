#pragma once

#include "../engine/engine.hpp"

class debug_cube : public cube {
  public:
    debug_cube(const shader *object_shader, const texture *tex,
               const texture *norm, double xpos, double ypos, double zpos);
    ~debug_cube();
};

inline debug_cube::debug_cube(const shader *object_shader, const texture *tex,
                              const texture *norm, double xpos, double ypos,
                              double zpos)
    : cube(object_shader, xpos, ypos, zpos) {
    this->add_texture(tex, "texture0");
    this->add_texture(norm, "normal0");
}

inline debug_cube::~debug_cube() {}
