#include "triangle.hpp"

#include "../utils/model_loader.hpp"


  static const std::vector<float> triangle_data = {
    -0.5f, -0.5f, -0.5f,
    - 0.5f, -0.5f, 0.5f,
    - 0.5f, 0.5f, -0.5f,
};

triangle::triangle(const shader *object_shader, double xpos, double ypos,
                   double zpos)
    : object(object_shader, model_loader::get().get_triangle(), xpos, ypos,
             zpos) {}


triangle::~triangle() {}

const std::vector<float> &triangle::get_triangle_data() {
  return triangle_data;
}


