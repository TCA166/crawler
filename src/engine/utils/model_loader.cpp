#include "model_loader.hpp"

#include <stdexcept>

static const std::vector<float> triangle_data = {
    // Bottom pyramid
    MODEL_LINE(0.0f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, 0.0f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    // Top pyramid
    MODEL_LINE(0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, 0.0f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
};

static const std::vector<unsigned int> triangle_indices = {
    0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1, 5, 6, 7, 5, 7, 8, 5, 8, 9, 5, 9, 6};

// Grass model data
static const std::vector<float> grass_data = {
    // Base left
    MODEL_LINE(-0.01f, 0.0f, 0.0f, 0.0f, 0.0f, -0.577f, -0.577f, 0.577f, 1.0f,
               0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
    // Base right
    MODEL_LINE(0.01f, 0.0f, 0.0f, 0.0f, 0.0f, -0.577f, 0.577f, -0.577f, -1.0f,
               0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
    // Base top
    MODEL_LINE(0.0f, 0.0f, 0.01f, 1.0f, 0.0f, 0.577f, 0.577f, -0.577f, -1.0f,
               0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
    // Tip
    MODEL_LINE(0.0f, 0.25f, -0.01f, 1.0f, 0.0f, -0.577f, 0.577f, 0.577f, 1.0f,
               0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
    // Tip
    MODEL_LINE(0.0f, 0.25f, 0.05f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 0.0f, 1.0f),
    // Tip
    MODEL_LINE(0.0f, 0.25f, -0.05f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
               1.0f, 0.0f, 0.0f, -1.0f),
};

static const std::vector<unsigned int> grass_indices = {
    0, 1, 2, 0, 2, 3, 0, 1, 3, 1, 2, 3, 0, 2, 4,
    0, 1, 4, 1, 2, 4, 0, 2, 5, 0, 1, 5, 1, 2, 5};

static const std::vector<float> cube_data = {
    MODEL_LINE(-.5f, -.5f, -.5f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f,
               0.f, 1.f),
    MODEL_LINE(.5f, -.5f, -.5f, 1.f, 0.f, -1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f,
               0.f, 1.f),
    MODEL_LINE(.5f, .5f, -.5f, 1.f, 1.f, -1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f,
               0.f, 1.f),
    MODEL_LINE(-.5f, .5f, -.5f, 0.f, 1.f, -1.f, 0.f, 0.f, -1.f, 1.f, 0.f, 0.f,
               0.f, 1.f),
    MODEL_LINE(-.5f, -.5f, .5f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f,
               0.f, 1.f),
    MODEL_LINE(.5f, -.5f, .5f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
               1.f),
    MODEL_LINE(.5f, .5f, .5f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
               1.f),
    MODEL_LINE(-.5f, .5f, .5f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f,
               1.f),
    MODEL_LINE(-.5f, -.5f, -.5f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
               1.f, 0.f),
    MODEL_LINE(-.5f, -.5f, .5f, 1.f, 0.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
               1.f, 0.f),
    MODEL_LINE(-.5f, .5f, .5f, 1.f, 1.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
               1.f, 0.f),
    MODEL_LINE(-.5f, .5f, -.5f, 0.f, 1.f, 0.f, -1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
               1.f, 0.f),
    MODEL_LINE(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
               1.f, 0.f),
    MODEL_LINE(.5f, -.5f, .5f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
               0.f),
    MODEL_LINE(.5f, .5f, .5f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
               0.f),
    MODEL_LINE(.5f, .5f, -.5f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
               0.f),
    MODEL_LINE(-.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f,
               0.f, 0.f),
    MODEL_LINE(.5f, -.5f, -.5f, 1.f, 0.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f,
               0.f, 0.f),
    MODEL_LINE(.5f, -.5f, .5f, 1.f, 1.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f,
               0.f, 0.f),
    MODEL_LINE(-.5f, -.5f, .5f, 0.f, 1.f, 0.f, 0.f, -1.f, 0.f, 1.f, 0.f, 1.f,
               0.f, 0.f),
    MODEL_LINE(-.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f,
               0.f, 0.f),
    MODEL_LINE(.5f, .5f, -.5f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f,
               0.f),
    MODEL_LINE(.5f, .5f, .5f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f,
               0.f),
    MODEL_LINE(-.5f, .5f, .5f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f,
               0.f)};

static const std::vector<unsigned int> cube_indices = {
    0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
    12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

static const std::vector<float> wall_data = {
    MODEL_LINE(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 0.0f),
    MODEL_LINE(0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 1.0f, 0.0f)};

static const std::vector<unsigned int> wall_indices = {0, 1, 2, 1, 3, 2};

// TODO check if the loading works fine with multiple contexts

// MAYBE model reference counting?

model_loader::model_loader()
    : cube(cube_data, cube_indices, glm::vec3(0.5), glm::vec3(-0.5)),
      triangle(triangle_data, triangle_indices, glm::vec3(-0.5, 0.5, 0.5),
               glm::vec3(-0.5, -0.5, -0.5)),

      wall(wall_data, wall_indices, glm::vec3(0.5, 0.5, 0.0),
           glm::vec3(-0.5, -0.5, 0.0)),
      grass(grass_data, grass_indices, glm::vec3(0.05, 1.0, 0.0),
            glm::vec3(-0.05, 0.0, 0.0)) {}

model_loader &model_loader::get() {
  static model_loader instance;
  return instance;
}

const model *model_loader::get_model(const std::string &key,
                                     uint32_t mesh_index) {
  std::pair<std::string, uint32_t> key_tuple = std::make_pair(key, mesh_index);
  try {
    return models.at(key_tuple);
  } catch (const std::out_of_range &e) {
#ifndef STATIC_ASSETS
    model *new_model = new model(key, mesh_index);
    models[key_tuple] = new_model;
    new_model->init();
    return new_model;
#else
    throw std::runtime_error("Model not found");
#endif
  }
}

const model *model_loader::get_cube() const { return &cube; }

const model *model_loader::get_triangle() const { return &triangle; }

const model *model_loader::get_grass() const { return &grass; }

const model *model_loader::get_wall() const { return &wall; }

void model_loader::init() {
  wall.init();
  cube.init();
  triangle.init();
  grass.init();
}

void model_loader::deinit() const {
  for (const auto &pair : models) {
    pair.second->deinit();
  }
}
