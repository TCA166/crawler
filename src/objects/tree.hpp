#pragma once

#include "../engine/engine.hpp"

#define BARK_POINTS 8
#define RING_POINTS (BARK_POINTS * 2)

#define MIN_BARK_RADIUS 0.2f
#define MAX_BARK_RADIUS 0.3f
#define BARK_VARIANCE 0.01f

#define MIN_SEGMENTS 20
#define MAX_SEGMENTS 30

#define SEGMENT_HEIGHT 0.1f

class random_tree : public object {
private:
  glm::vec3 bark_bound, bark_negbound;
  uint8_t bark_segment_count;
  model tree;
  texture tex, norm;

public:
  random_tree(const shader *object_shader, double xpos, double ypos,
              double zpos);
  ~random_tree();
};

static inline std::vector<float> generate_data(uint8_t segments) {
  // https://math.stackexchange.com/questions/4459356/find-n-evenly-spaced-points-on-circle-with-radius-r
  std::vector<glm::vec2> ring_points;
  for (uint8_t i = 0; i < RING_POINTS; i++) {
    float angle = (2 * M_PI * i) / RING_POINTS;
    ring_points.push_back(glm::vec2(cos(angle), sin(angle)));
  }
  float base_radius = glm::linearRand(MIN_BARK_RADIUS, MAX_BARK_RADIUS);
  std::vector<glm::vec3> points;
  for (uint8_t i = 0; i < segments; i++) {
    float radius = base_radius + glm::linearRand(-BARK_VARIANCE, BARK_VARIANCE);
    for (size_t j = 0; j < ring_points.size(); j++) {
      points.push_back(glm::vec3(radius * ring_points[j].x, i * SEGMENT_HEIGHT,
                                 radius * ring_points[j].y));
    }
  }
  points.push_back(glm::vec3(0.0f, segments * SEGMENT_HEIGHT * 2, 0.0f));
  std::vector<float> data;
  for (size_t i = 0; i < points.size(); i++) {
    data.push_back(points[i].x);
    data.push_back(points[i].y);
    data.push_back(points[i].z);
    // texture
    data.push_back(points[i].x);
    data.push_back(points[i].z);
    // normal
    data.push_back(0.0);
    data.push_back(1.0);
    data.push_back(0.0);
    // tangent
    data.push_back(1.0);
    data.push_back(0.0);
    data.push_back(0.0);
    // bitangent
    data.push_back(0.0);
    data.push_back(0.0);
    data.push_back(1.0);
  }
  return data;
}

static inline std::vector<unsigned int> generate_indices(uint8_t segments) {
  std::vector<unsigned int> indices;
  for (uint8_t y = 0; y < segments - 1; y++) {
    for (uint8_t x = 0; x < RING_POINTS; x++) {
      indices.push_back(y * RING_POINTS + x);
      indices.push_back(y * RING_POINTS + (x + 1) % RING_POINTS);
      indices.push_back((y + 1) * RING_POINTS + x);
      indices.push_back((y + 1) * RING_POINTS + x);
      indices.push_back(y * RING_POINTS + (x + 1) % RING_POINTS);
      indices.push_back((y + 1) * RING_POINTS + (x + 1) % RING_POINTS);
    }
  }
  for (uint8_t x = 0; x < RING_POINTS; x++) {
    indices.push_back((segments - 1) * RING_POINTS + x);
    indices.push_back((segments - 1) * RING_POINTS + (x + 1) % RING_POINTS);
    indices.push_back(segments * RING_POINTS);
  }
  return indices;
}

inline random_tree::random_tree(const shader *object_shader, double xpos,
                                double ypos, double zpos)
    : object(object_shader, &tree, xpos, ypos, zpos),
      bark_segment_count(glm::linearRand(MIN_SEGMENTS, MAX_SEGMENTS)),
      tree(generate_data(bark_segment_count),
           generate_indices(bark_segment_count), glm::vec3(0.0f),
           glm::vec3(0.0f)),
      tex(TEXTURE_PATH("poplar.jpg")), norm(TEXTURE_PATH("poplar_normal.jpg")) {
  tree.init();
  bark_bound = glm::vec3(0.5, 1.0, 0.5);
  bark_negbound = glm::vec3(-0.5, 0.0, -0.5);
  add_texture(&tex, "texture0");
  add_texture(&norm, "normal0");
}

inline random_tree::~random_tree() {}
