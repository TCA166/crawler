#include "wall.hpp"

// clang-format off

static const std::vector<float> wall_data = {
    // positions          // normals           // texture coords // tangent           // bitangent
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};

static const std::vector<unsigned int> wall_indices = {
    0, 1, 2, 1, 3, 2
};

// clang-format on

wall::wall(const shader *object_shader, double xpos, double ypos, double zpos)
    : object(object_shader, wall_data, wall_indices, glm::vec3(0.5, 0.5, 0.0),
             glm::vec3(-0.5, -0.5, 0.0), xpos, ypos, zpos) {}

wall::~wall() {}
