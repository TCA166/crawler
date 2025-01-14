#include "triangle.hpp"

// clang-format off

static const std::vector<float> triangle_data = {
    // positions          // normals           // texture coords // tangent           // bitangent
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};

static const std::vector<unsigned int> triangle_indices = {0, 1, 2};

// clang-format on

triangle::triangle(const shader *object_shader, double xpos, double ypos,
                   double zpos)
    : object(object_shader, triangle_data, triangle_indices, 0.5, -0.5, 0.5,
             -0.5, 0.5, -0.5, xpos, ypos, zpos) {}

triangle::~triangle() {}
