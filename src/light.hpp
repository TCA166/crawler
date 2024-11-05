
#pragma once

#include "include.hpp"

struct light {
    glm::vec3 position;
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;
};
