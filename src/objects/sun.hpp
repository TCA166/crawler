
#pragma once

#include "../game_object.hpp"
#include "../physics/constants.hpp"
#include "../engine/light.hpp"

class sun : public game_object, public light {
    public:
        sun(shader* target_shader, double xpos, double ypos, double zpos);
        virtual ~sun();
        glm::vec3 get_position() const {
            return glm::vec3(xpos, ypos, zpos);
        }
};

inline sun::sun(shader* target_shader, double xpos, double ypos, double zpos) : game_object(target_shader, "models/earth.obj", sun_mass, glm::vec3(0.0), xpos, ypos, zpos), light(glm::vec3(xpos, ypos, zpos), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f){
    this->set_scale(sun_radius / earth_radius);
}

inline sun::~sun(){
    
}
