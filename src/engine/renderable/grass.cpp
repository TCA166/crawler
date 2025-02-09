#include "grass.hpp"

#include "../utils/model_loader.hpp"

grass::grass(double xpos, double ypos,
                   double zpos)
    : object(model_loader::get().get_grass(), xpos, ypos,
             zpos) {}

grass::~grass() {}
