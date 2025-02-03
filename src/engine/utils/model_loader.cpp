#include "model_loader.hpp"

#include <stdexcept>

static const std::vector<float> triangle_data = {
    MODEL_LINE(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
    MODEL_LINE(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f),
    MODEL_LINE(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f)};

static const std::vector<unsigned int> triangle_indices = {0, 1, 2};

static const std::vector<float> cube_data = {
    MODEL_LINE(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
    MODEL_LINE(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f),
    MODEL_LINE(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f),
    MODEL_LINE(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, -0.5f, -0.5f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, -0.5f, 0.5f, 1.0f, 1.0f),
    MODEL_LINE(0.5f, 0.5f, -0.5f, 0.0f, 0.0f),
    MODEL_LINE(0.5f, 0.5f, 0.5f, 0.0f, 1.0f),
};

static const std::vector<unsigned int> cube_indices = {
    0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 0, 1, 4, 1, 4, 5,
    2, 3, 6, 3, 6, 7, 0, 2, 4, 2, 4, 6, 1, 3, 5, 3, 5, 7};

static const std::vector<float> wall_data = {
    MODEL_LINE(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
    MODEL_LINE(0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f),
    MODEL_LINE(0.5f, 0.5f, 0.0f, 1.0f, 1.0f),
};

static const std::vector<unsigned int> wall_indices = {0, 1, 2, 1, 3, 2};

// TODO check if the loading works fine with multiple contexts

// MAYBE model reference counting?

model_loader::model_loader()
    : cube(cube_data, cube_indices, glm::vec3(0.5), glm::vec3(-0.5)),
      triangle(triangle_data, triangle_indices, glm::vec3(-0.5, 0.5, 0.5),
               glm::vec3(-0.5, -0.5, -0.5)),
      wall(wall_data, wall_indices, glm::vec3(0.5, 0.5, 0.0),
           glm::vec3(-0.5, -0.5, 0.0)) {}

model_loader &model_loader::get() {
  static model_loader instance;
  return instance;
}

const model *model_loader::get_model(const std::string &key) {
  try {
    return models.at(key);
  } catch (const std::out_of_range &e) {
#ifndef STATIC_ASSETS
    model *new_model = new model(key);
    models[key] = new_model;
    return new_model;
#else
    throw std::runtime_error("Model not found");
#endif
  }
}

const model *model_loader::get_cube() const { return &cube; }

const model *model_loader::get_triangle() const { return &triangle; }

const model *model_loader::get_wall() const { return &wall; }

void model_loader::init() {
  wall.init();
  cube.init();
  triangle.init();
  for (auto pair : models) {
    pair.second->init();
  }
}

void model_loader::deinit() const {
  for (const auto &pair : models) {
    pair.second->deinit();
  }
}
