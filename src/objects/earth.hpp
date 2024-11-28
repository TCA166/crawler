
#pragma once

#include "../game_object.hpp"
#include "../physics/constants.hpp"

class earth : public game_object {
    private:
        texture ground, ground_normal, night, clouds, spec;
        shader* target_shader;
    public:
        earth(shader* target_shader, double xpos, double ypos, double zpos);
        virtual ~earth();
};

inline earth::earth(shader* target_shader, double xpos, double ypos, double zpos) : game_object(target_shader, "models/earth.obj", earth_mass, glm::vec3(0.0, 0.0, INITIAL_VELOCITY(sun_mass, earth_sun_distance)), xpos, ypos, zpos), ground(texture("textures/ground.jpg")), ground_normal(texture("textures/ground_normal.png")), night(texture("textures/night.jpg")), clouds(texture("textures/clouds.png")), spec(texture("textures/spec.png")) {
    this->add_texture(&ground, "ground");
    this->add_texture(&ground_normal, "ground_normal");
    this->add_texture(&night, "night");
    this->add_texture(&clouds, "clouds");
    this->add_texture(&spec, "spec");
}

inline earth::~earth(){
    
}
