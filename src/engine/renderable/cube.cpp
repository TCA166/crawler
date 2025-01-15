#include "cube.hpp"

#include "../utils/model_loader.hpp"

cube::cube(const shader *object_shader, double xpos, double ypos, double zpos)
    : object(object_shader, model_loader::get().get_cube(), xpos, ypos, zpos) {}

cube::~cube() {}
