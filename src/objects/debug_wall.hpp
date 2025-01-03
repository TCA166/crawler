#pragma once

#include "../engine/engine.hpp"

class debug_wall : public wall {
  private:
    texture *wall_texture, *wall_normal;

  public:
    debug_wall(const shader *object_shader, double xpos, double ypos,
               double zpos);
    ~debug_wall();
};

inline debug_wall::debug_wall(const shader *object_shader, double xpos,
                              double ypos, double zpos)
    : wall(object_shader, xpos, ypos, zpos) {
    wall_texture = new texture(TEXTURE_PATH("spaceship.jpg"));
    wall_normal = new texture(TEXTURE_PATH("spaceship_normal.jpg"));
    this->add_texture(wall_texture, "texture0");
    this->add_texture(wall_normal, "normal0");
}

inline debug_wall::~debug_wall() {
    delete wall_texture;
    delete wall_normal;
}
