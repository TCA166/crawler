
#pragma once

#include "../game_object.hpp"
#include "../physics/constants.hpp"

class moon : public game_object {
    private:
        texture ground, ground_normal;
        shader* target_shader;
    public:
        moon(shader* target_shader, double xpos, double ypos, double zpos);
        virtual ~moon();
};

//FIXME the moon is not stable
inline moon::moon(shader* target_shader, double xpos, double ypos, double zpos) : game_object(target_shader, "models/earth.obj", moon_mass, glm::vec3(0.0, 0.0, INITIAL_VELOCITY(earth_mass, earth_moon_distance)), xpos, ypos, zpos), ground(texture("textures/moon.png")), ground_normal(texture("textures/moon_normal.png")){
    this->add_texture(&ground, "texture0");
    this->add_texture(&ground_normal, "normal0");
    this->set_scale(moon_radius / earth_radius);
}

inline moon::~moon(){
    
}
