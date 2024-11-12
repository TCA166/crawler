
#pragma once

#include "../engine.hpp"

class ship : public object {
    private:
        texture body, normal;
        shader* textured_shader;
    public:
        ship(shader* textured_shader, double xpos, double ypos, double zpos);
        ~ship();
};
