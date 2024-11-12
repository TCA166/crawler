
#pragma once

#include "../engine.hpp"

class earth : public object {
    private:
        texture ground, ground_normal, night, clouds;
        shader* target_shader;
    public:
        earth(shader* target_shader, double xpos, double ypos, double zpos);
        ~earth();
};
