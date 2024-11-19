
#pragma once

#include "include.hpp"

class light {
    private:
        glm::vec3 position;
        glm::vec3 color;
        float constant;
        float linear;
        float quadratic;
    public:
        light(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic);
        virtual ~light();
        const glm::vec3* get_position() const;
        const glm::vec3* get_color() const;
        float get_constant() const;
        float get_linear() const;
        float get_quadratic() const;
};
