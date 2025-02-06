#pragma once

#include "../engine/engine.hpp"

#define BARK_POINTS 8
#define RING_POINTS (BARK_POINTS * 2)

/*!
 @brief A procedurally generated model of a tree branch (cylinder with tip)
*/
class branch : public model {
public:
  /*!
   @brief Constructs a branch object
   @param num_segments The number of segments in the branch
   @param segment_height The height of each segment
   @param root_radius The radius of the root of the branch
   @param variance The variance of the branch
   @param tip_offset The offset of the tip of the branch
  */
  branch(uint8_t num_segments, float segment_height, float root_radius,
         float variance, float tip_offset);
  ~branch();
};

inline branch::branch(uint8_t num_segments, float segment_height,
                      float root_radius, float variance, float tip_offset) {
  // https://math.stackexchange.com/questions/4459356/find-n-evenly-spaced-points-on-circle-with-radius-r
  std::vector<glm::vec2> ring_points; // first we generate a flat ring
  for (uint8_t i = 0; i < RING_POINTS; i++) {
    float angle = (2 * M_PI * i) / RING_POINTS;
    ring_points.push_back(glm::vec2(cos(angle), sin(angle)));
  }
  // then we copy this ring for each segment
  std::vector<glm::vec3> points;
  for (uint8_t i = 0; i < num_segments; i++) {
    float radius = root_radius + glm::linearRand(-variance, variance);
    for (size_t j = 0; j < ring_points.size(); j++) {
      points.push_back(glm::vec3(radius * ring_points[j].x, i * segment_height,
                                 radius * ring_points[j].y));
    }
  }
  negbounds = glm::vec3(-root_radius, 0.0, -root_radius);
  float tip_y = num_segments * segment_height + tip_offset;
  bounds = glm::vec3(root_radius, tip_y, root_radius);
  // add a tip
  points.push_back(glm::vec3(0.0f, tip_y, 0.0f));
  // then we use the rings to generate the bark
  for (size_t i = 0; i < points.size(); i++) {
    data.push_back(points[i].x);
    data.push_back(points[i].y);
    data.push_back(points[i].z);
    uint8_t ring_index = i % RING_POINTS;
    float angle = (2 * M_PI * ring_index) / RING_POINTS;
    // texture
    data.push_back(angle); // we wrap the texture around the bark
    data.push_back(points[i].y);
    // normal
    // a normal is meant to be perpendicular to the tangent of the circle
    data.push_back(cos(angle));
    data.push_back(0.0);
    data.push_back(sin(angle));
    // tangent
    data.push_back(-sin(angle));
    data.push_back(0.0);
    data.push_back(cos(angle));
    // bitangent
    data.push_back(0.0);
    data.push_back(1.0);
    data.push_back(0.0);
  }
  // indices
  for (uint8_t y = 0; y < num_segments - 1; y++) {
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
    indices.push_back((num_segments - 1) * RING_POINTS + x);
    indices.push_back((num_segments - 1) * RING_POINTS + (x + 1) % RING_POINTS);
    indices.push_back(num_segments * RING_POINTS);
  }
}

inline branch::~branch() {}

#define MIN_BARK_RADIUS 0.2f
#define MAX_BARK_RADIUS 0.3f
#define BARK_VARIANCE 0.01f

#define BARK_HEIGHT 2.f
#define SEGMENT_HEIGHT .1f

#define MIN_SEGMENTS (BARK_HEIGHT / SEGMENT_HEIGHT)
#define MAX_SEGMENTS MIN_SEGMENTS * 1.5f

class random_tree : public object {
private:
  branch tree;
  texture tex, norm;

public:
  random_tree(const shader *object_shader, double xpos, double ypos,
              double zpos);
  ~random_tree();
};

inline random_tree::random_tree(const shader *object_shader, double xpos,
                                double ypos, double zpos)
    : object(object_shader, &tree, xpos, ypos, zpos),
      tree(glm::linearRand(MIN_SEGMENTS, MAX_SEGMENTS), SEGMENT_HEIGHT,
           glm::linearRand(MIN_BARK_RADIUS, MAX_BARK_RADIUS), BARK_VARIANCE,
           0.5),
      tex(TEXTURE_PATH("poplar.jpg")), norm(TEXTURE_PATH("poplar_normal.jpg")) {
  tree.init();
  add_texture(&tex, "texture0");
  add_texture(&norm, "normal0");
}

inline random_tree::~random_tree() {}
