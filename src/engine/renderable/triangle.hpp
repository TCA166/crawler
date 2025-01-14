
#pragma once

#include "object.hpp"

class triangle : public object {
public:
  triangle(const shader *object_shader, double xpos, double ypos, double zpos);
  ~triangle();
};
