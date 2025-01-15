#include "triangle.hpp"

#include "../utils/model_loader.hpp"

triangle::triangle(const shader *object_shader, double xpos, double ypos,
                   double zpos)
    : object(object_shader, model_loader::get().get_triangle(), xpos, ypos,
             zpos) {}

triangle::~triangle() {}
