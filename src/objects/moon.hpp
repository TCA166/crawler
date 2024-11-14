
#pragma once

#include "../engine.hpp"

class moon : public object {
    private:
        texture ground, ground_normal;
        shader* target_shader;
    public:
        moon(shader* target_shader, double xpos, double ypos, double zpos);
        ~moon();
};

inline moon::moon(shader* target_shader, double xpos, double ypos, double zpos) : object(target_shader, "models/earth.obj", xpos, ypos, zpos), ground(texture("textures/moon.png")), ground_normal(texture("textures/moon_normal.png")){
    this->add_texture(&ground, "texture0");
    this->add_texture(&ground_normal, "normal0");
    this->set_scale(0.27);
}

inline moon::~moon(){
    
}
