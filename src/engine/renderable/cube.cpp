#include "cube.hpp"

// x, y, z, nx, ny, nz, tx, ty, tanx, tany, tanz, bitanx, bitany, bitanz

static const std::vector<float> cube_data = {
    // positions          // normals           // texture coords // tangent           // bitangent
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,       -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,       -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,       -1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};

static const std::vector<unsigned int> cube_indices = {
    0, 1, 2, 1, 2, 3,
    4, 5, 6, 5, 6, 7,
    0, 1, 4, 1, 4, 5,
    2, 3, 6, 3, 6, 7,
    0, 2, 4, 2, 4, 6,
    1, 3, 5, 3, 5, 7
};

cube::cube(const shader* object_shader, double xpos, double ypos, double zpos) : object(object_shader, cube_data, cube_indices, xpos, ypos, zpos) {

}

cube::~cube() {
    
}
