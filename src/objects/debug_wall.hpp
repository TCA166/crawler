#pragma once

#include "../engine/engine.hpp"

class debug_wall : public wall {
public:
  debug_wall(const shader *object_shader, const texture *tex,
             const texture *norm, double xpos, double ypos, double zpos);
  ~debug_wall();
};

inline debug_wall::debug_wall(const shader *object_shader, const texture *tex,
                              const texture *norm, double xpos, double ypos,
                              double zpos)
    : wall(object_shader, xpos, ypos, zpos) {
  this->add_texture(tex, "texture0");
  this->add_texture(norm, "normal0");
}

inline debug_wall::~debug_wall() {}
