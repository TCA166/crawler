#include "wall.hpp"

#include "../utils/model_loader.hpp"

wall::wall(const shader *object_shader, double xpos, double ypos, double zpos)
    : object(object_shader, model_loader::get().get_wall(), xpos, ypos, zpos) {}

wall::~wall() {}
