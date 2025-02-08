#include "grass.hpp"

#include "../utils/model_loader.hpp"

grass::grass(const shader *object_shader, double xpos, double ypos,
                   double zpos)
    : object(object_shader, model_loader::get().get_grass(), xpos, ypos,
             zpos) {}

grass::~grass() {}
