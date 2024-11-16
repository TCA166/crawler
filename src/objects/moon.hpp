
#pragma once

#include "../game_object.hpp"
#include "../physics/constants.hpp"

#define INITIAL_VELOCITY sqrt(GRAVITATIONAL_CONSTANT * EARTH_MASS / EARTH_MOON_DISTANCE)

class moon : public game_object {
    private:
        texture ground, ground_normal;
        shader* target_shader;
    public:
        moon(shader* target_shader, double xpos, double ypos, double zpos);
        virtual ~moon();
};

// FIXME the moon flies into the space, it should orbit the earth, pass earth as an argument?
inline moon::moon(shader* target_shader, double xpos, double ypos, double zpos) : game_object(target_shader, "models/earth.obj", MOON_MASS, glm::vec3(0.0, 0.0, INITIAL_VELOCITY), xpos, ypos, zpos), ground(texture("textures/moon.png")), ground_normal(texture("textures/moon_normal.png")){
    this->add_texture(&ground, "texture0");
    this->add_texture(&ground_normal, "normal0");
    this->set_scale(MOON_RADIUS / EARTH_RADIUS);
}

inline moon::~moon(){
    
}
