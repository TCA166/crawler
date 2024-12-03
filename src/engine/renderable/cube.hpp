
#pragma once

#include "object.hpp"

class cube : public object {
    public:
        cube(const shader* object_shader, double xpos, double ypos, double zpos);
        ~cube();
};
