
#pragma once

#include "engine.hpp"
#include "physics/entity.hpp"

class game_object : public object, public entity {
    public:
        game_object(const shader* object_shader, const std::string& path, double mass, glm::vec3 velocity, double xpos, double ypos, double zpos);
        ~game_object();
        void evaluate(double delta_time);
};
