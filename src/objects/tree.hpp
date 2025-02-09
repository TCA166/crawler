#pragma once

#include <random>

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"

// the number of points the curve of the cylinder is defined by
#define BARK_POINTS 8
#define RING_POINTS (BARK_POINTS * 2)
// the radius of the root of the branch
#define BRANCH_RADIUS 0.05f
#define BRANCH_MAX 2
#define BRANCH_MIN_LENGTH 0.5f
#define BRANCH_MAX_LENGTH 1.2f
#define BRANCH_SEGMENTS 2

/*!
 @brief A procedurally generated model of a tree
*/
class tree_model : public model {
private:
  std::vector<std::pair<glm::vec3, glm::vec3>> branch_points;

public:
  /*!
   @brief Constructs a new tree model
   @param num_segments The number of segments in the branch
   @param segment_height The height of each segment
   @param root_radius The radius of the root of the branch
   @param variance The variance of the branch
   @param tip_offset The offset of the tip of the branch
  */
  tree_model(uint8_t num_segments, float segment_height, float root_radius,
             float variance, float tip_offset);
  ~tree_model();
  /*!
   @brief Gets the points constituting branches on the logical level
   @return A vector of point pairs
  */
  const std::vector<std::pair<glm::vec3, glm::vec3>> &get_branch_points() const;
};

static inline void add_data(std::vector<float> &data, glm::vec3 vertex,
                            glm::vec2 texel, glm::vec3 normal,
                            glm::vec3 tangent, glm::vec3 bitangent) {
  data.push_back(vertex.x);
  data.push_back(vertex.y);
  data.push_back(vertex.z);
  data.push_back(texel.x);
  data.push_back(texel.y);
  data.push_back(normal.x);
  data.push_back(normal.y);
  data.push_back(normal.z);
  data.push_back(tangent.x);
  data.push_back(tangent.y);
  data.push_back(tangent.z);
  data.push_back(bitangent.x);
  data.push_back(bitangent.y);
  data.push_back(bitangent.z);
}

inline tree_model::tree_model(uint8_t num_segments, float segment_height,
                              float root_radius, float variance,
                              float tip_offset) {
  // https://math.stackexchange.com/questions/4459356/find-n-evenly-spaced-points-on-circle-with-radius-r
  std::vector<glm::vec2> ring_points; // first we generate a flat ring
  for (uint8_t i = 0; i < RING_POINTS; i++) {
    float angle = (2 * M_PI * i) / RING_POINTS;
    ring_points.push_back(glm::vec2(cos(angle), sin(angle)));
  }
  // future branch definitions
  // then we copy this ring for each segment
  std::vector<glm::vec3> points;
  for (uint8_t i = 0; i < num_segments; i++) {
    float radiance = glm::linearRand(-variance, variance);
    float radius = root_radius + radiance;
    root_radius += radiance;
    for (size_t j = 0; j < ring_points.size(); j++) {
      points.push_back(glm::vec3(radius * ring_points[j].x, i * segment_height,
                                 radius * ring_points[j].y));
    }
    for (uint8_t b = 0; b < glm::linearRand(0, BRANCH_MAX); b++) {
      // prob check weighted by height
      if (glm::linearRand(0.f, 1.f) < ((float)i / (float)(num_segments + b))) {
        float angle = glm::linearRand(0., 2. * M_PI);
        float start_radius = radius * 0.9;
        float cos_angle = cosf(angle);
        float sin_angle = sinf(angle);
        float end_radius =
            glm::linearRand(BRANCH_MIN_LENGTH, BRANCH_MAX_LENGTH) + radius;

        branch_points.push_back(std::make_pair(
            glm::vec3(start_radius * cos_angle, (i + 1) * segment_height,
                      start_radius * sin_angle),
            glm::vec3(end_radius * cos_angle,
                      (glm::linearRand(-1.f, 1.f) * segment_height) +
                          (i * segment_height),
                      end_radius * sin_angle)));
      }
    }
  }
  // then we use the rings to generate the bark
  for (size_t i = 0; i < points.size(); i++) {
    uint8_t ring_index = i % RING_POINTS;
    float angle = (2 * M_PI * ring_index) / RING_POINTS;
    add_data(data, points[i], glm::vec2(angle, points[i].y),
             glm::vec3(cosf(angle), 0., sinf(angle)),
             glm::vec3(-sinf(angle), 0.f, cosf(angle)),
             glm::vec3(0.f, 1.f, 0.f));
  }
  // indices
  for (uint8_t y = 0; y < num_segments - 1; y++) {
    uint32_t point_off = y * RING_POINTS;
    for (uint8_t x = 0; x < RING_POINTS; x++) {
      indices.push_back(point_off + x);
      indices.push_back(point_off + (x + 1) % RING_POINTS);
      indices.push_back(point_off + RING_POINTS + x);
      indices.push_back(point_off + RING_POINTS + x);
      indices.push_back(point_off + (x + 1) % RING_POINTS);
      indices.push_back(point_off + RING_POINTS + (x + 1) % RING_POINTS);
    }
  }
  float tip_y = num_segments * segment_height + tip_offset;
  negbounds = glm::vec3(-root_radius, 0.0, -root_radius);
  bounds = glm::vec3(root_radius, tip_y, root_radius);
  // tip
  add_data(data, glm::vec3(0.f, tip_y, 0.f), glm::vec2(0.f, tip_y),
           glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f),
           glm::vec3(0.f, 0.f, 1.f));
  for (uint8_t x = 0; x < RING_POINTS; x++) {
    indices.push_back((num_segments - 1) * RING_POINTS + x);
    indices.push_back((num_segments - 1) * RING_POINTS + (x + 1) % RING_POINTS);
    indices.push_back(num_segments * RING_POINTS);
  }
  // branches
  for (size_t i = 0; i < branch_points.size(); i++) {
    glm::vec3 branch_start = branch_points[i].first;
    glm::vec3 branch_end = branch_points[i].second;
    uint32_t point_off =
        points.size() + 1 + (i * (BARK_POINTS * BRANCH_SEGMENTS + 1));
    // generate a cylinder around the line between the two points
    glm::vec3 direction = branch_end - branch_start;
    glm::vec3 tangent = glm::normalize(direction);
    glm::vec3 bitangent = glm::normalize(glm::cross(tangent, UP));
    glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));
    for (uint8_t seg = 0; seg < BRANCH_SEGMENTS; seg++) {
      for (uint8_t j = 0; j < BARK_POINTS; j++) {
        float angle = (2 * M_PI * j) / BARK_POINTS;
        glm::vec3 point = branch_start +
                          (direction * (seg / (float)BRANCH_SEGMENTS)) +
                          BRANCH_RADIUS * cosf(angle) * bitangent +
                          BRANCH_RADIUS * sinf(angle) * normal;
        glm::vec3 normal_seg =
            glm::normalize(cosf(angle) * bitangent + sinf(angle) * normal);

        add_data(data, point, glm::vec2(angle, seg), normal_seg, tangent,
                 bitangent);
      }
      for (uint8_t j = 0; j < BARK_POINTS;
           j++) { // FIXME this doesn't work well with BARK_POINTS > 2
        indices.push_back(point_off + j);
        indices.push_back(point_off + (j + 1) % BARK_POINTS);
        indices.push_back(point_off + BARK_POINTS + j);
        indices.push_back(point_off + BARK_POINTS + j);
        indices.push_back(point_off + (j + 1) % BARK_POINTS);
        indices.push_back(point_off + BARK_POINTS + (j + 1) % BARK_POINTS);
      }
    }
    add_data(data, branch_end, glm::vec2(0.f, glm::length(direction)), normal,
             tangent, bitangent);
    point_off += (BARK_POINTS) * (BRANCH_SEGMENTS - 1);
    for (uint8_t j = 0; j < BARK_POINTS; j++) {
      indices.push_back(point_off + j);
      indices.push_back(point_off + (j + 1) % BARK_POINTS);
      indices.push_back(point_off + BARK_POINTS);
    }
  }
}

inline tree_model::~tree_model() {}

inline const std::vector<std::pair<glm::vec3, glm::vec3>> &
tree_model::get_branch_points() const {
  return branch_points;
}

#define MIN_BARK_RADIUS 0.2f
#define MAX_BARK_RADIUS 0.3f
#define BARK_VARIANCE 0.01f

#define BARK_HEIGHT 2.f
#define SEGMENT_HEIGHT .1f

#define MIN_SEGMENTS (BARK_HEIGHT / SEGMENT_HEIGHT)
#define MAX_SEGMENTS MIN_SEGMENTS * 1.5f

#define MIN_TIP_Y 0.5f
#define MAX_TIP_Y 1.5f

/*!
 @brief A procedurally generated model of a tree
*/
class random_tree : public object {
private:
  uint8_t segment_count;
  float tip_y;
  tree_model tree;
  texture tex, norm;

public:
  /*!
   @brief Constructs a random tree object
   @param xpos The x position of the tree
   @param ypos The y position of the tree
   @param zpos The z position of the tree
  */
  random_tree(double xpos, double ypos, double zpos);
  ~random_tree();
  /*!
   @brief Gets the points constituting leaves on the logical level
   @return A vector of point pairs
  */
  const std::vector<std::pair<glm::vec3, glm::vec3>> &get_leaves_points() const;
  /*!
   @brief Gets the y position of the tip of the tree
   @return The y position of the tip of the tree
  */
  float get_tip_y() const;
};

inline random_tree::random_tree(double xpos, double ypos, double zpos)
    : object(&tree, xpos, ypos, zpos),
      segment_count(glm::linearRand(MIN_SEGMENTS, MAX_SEGMENTS)),
      tip_y(glm::linearRand(MIN_TIP_Y, MAX_TIP_Y)),
      tree(segment_count, SEGMENT_HEIGHT,
           glm::linearRand(MIN_BARK_RADIUS, MAX_BARK_RADIUS), BARK_VARIANCE,
           tip_y),
      tex(TEXTURE_PATH("poplar.jpg")), norm(TEXTURE_PATH("poplar_normal.jpg")) {
  tree.init();
  add_texture(&tex, "texture0");
  add_texture(&norm, "normal0");
}

inline random_tree::~random_tree() {}

inline const std::vector<std::pair<glm::vec3, glm::vec3>> &
random_tree::get_leaves_points() const {
  return tree.get_branch_points();
}

inline float random_tree::get_tip_y() const { return tip_y; }
