#include "triangle.hpp"

#include "../utils/model_loader.hpp"

triangle::triangle(const shader *object_shader,
                   const std::string &model_path, double xpos, double ypos,
                   double zpos)
    : object(object_shader, model_loader::get().get_model(model_path), xpos, ypos,
             zpos) {}

triangle::~triangle() {}
