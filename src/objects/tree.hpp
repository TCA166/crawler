#pragma once

#include <random>

#include "../engine/engine.hpp"
#include "../engine/utils/model_loader.hpp"
#include "../engine/utils/noise.hpp"

// the number of points the curve of the cylinder is defined by
#define BARK_POINTS 8
#define RING_POINTS (BARK_POINTS * 2)
// the radius of the root of the branch
#define BRANCH_RADIUS 0.05f
#define BRANCH_MAX 2
#define BRANCH_MIN_LENGTH 0.5f
#define BRANCH_MAX_LENGTH 1.2f
#define BRANCH_SEGMENTS 2

// TODO add leaves

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

inline branch::branch(uint8_t num_segments, float segment_height,
                      float root_radius, float variance, float tip_offset) {
  // https://math.stackexchange.com/questions/4459356/find-n-evenly-spaced-points-on-circle-with-radius-r
  std::vector<glm::vec2> ring_points; // first we generate a flat ring
  for (uint8_t i = 0; i < RING_POINTS; i++) {
    float angle = (2 * M_PI * i) / RING_POINTS;
    ring_points.push_back(glm::vec2(cos(angle), sin(angle)));
  }
  // future branch definitions
  std::vector<glm::vec3> branch_points;
  std::vector<glm::vec3> branch_end_points;
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
        branch_points.push_back(glm::vec3(start_radius * cos_angle,
                                          (i + 1) * segment_height,
                                          start_radius * sin_angle));
        float end_radius =
            glm::linearRand(BRANCH_MIN_LENGTH, BRANCH_MAX_LENGTH) + radius;
        branch_end_points.push_back(
            glm::vec3(end_radius * cos_angle,
                      (glm::linearRand(-1.f, 1.f) * segment_height) +
                          (i * segment_height),
                      end_radius * sin_angle));
      }
    }
  }
  negbounds = glm::vec3(-root_radius, 0.0, -root_radius);
  float tip_y = num_segments * segment_height + tip_offset;
  bounds = glm::vec3(root_radius, tip_y, root_radius);
  // add a tip
  points.push_back(glm::vec3(0.0f, tip_y, 0.0f));
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
  for (uint8_t x = 0; x < RING_POINTS; x++) {
    indices.push_back((num_segments - 1) * RING_POINTS + x);
    indices.push_back((num_segments - 1) * RING_POINTS + (x + 1) % RING_POINTS);
    indices.push_back(num_segments * RING_POINTS);
  }
  // branches
  for (size_t i = 0; i < branch_points.size(); i++) {
    uint32_t point_off =
        points.size() + (i * (BARK_POINTS * BRANCH_SEGMENTS + 1));
    // generate a cylinder around the line between the two points
    glm::vec3 direction = branch_end_points[i] - branch_points[i];
    glm::vec3 tangent = glm::normalize(direction);
    glm::vec3 bitangent = glm::normalize(glm::cross(tangent, UP));
    glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));
    for (uint8_t seg = 0; seg < BRANCH_SEGMENTS; seg++) {
      for (uint8_t j = 0; j < BARK_POINTS; j++) {
        float angle = (2 * M_PI * j) / BARK_POINTS;
        glm::vec3 point = branch_points[i] +
                          (direction * (seg / (float)BRANCH_SEGMENTS)) +
                          BRANCH_RADIUS * cosf(angle) * bitangent +
                          BRANCH_RADIUS * sinf(angle) * normal;
        add_data(data, point, glm::vec2(angle, seg),
                 glm::vec3(cosf(angle), 0., sinf(angle)),
                 glm::vec3(-sinf(angle), 0.f, cosf(angle)), bitangent);
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
    add_data(data, branch_end_points[i], glm::vec2(0.f, glm::length(direction)),
             normal, tangent, bitangent);
    point_off += (BARK_POINTS) * (BRANCH_SEGMENTS - 1);
    for (uint8_t j = 0; j < BARK_POINTS; j++) {
      indices.push_back(point_off + j);
      indices.push_back(point_off + (j + 1) % BARK_POINTS);
      indices.push_back(point_off + BARK_POINTS);
    }
  }
}

inline branch::~branch() {}

inline texture *create_random_leaf_texture(uint32_t size,
                                           uint8_t color_variance) {
  uint8_t *leaf_data = new uint8_t[size * size * 4];
  for (size_t i = 0; i < size * size; i += 4) {
    leaf_data[i] = 0;
    leaf_data[i + 1] =
        (255 - color_variance) + glm::linearRand((uint8_t)0, color_variance);
    leaf_data[i + 2] = 0;
    float noise_val = noise((i % size), (i / size)) + 1.f * 128.f;
    leaf_data[i + 3] = (uint8_t)noise_val; // alpha
  }
  image_t leaf_image = {leaf_data, size, size, 4};
  return new texture(&leaf_image);
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

#define LEAF_MAX_SIZE 1.5f

class random_tree : public object {
private:
  uint8_t segment_count;
  float tip_y;
  branch tree;
  texture tex, norm;
  std::list<object *> leaves;
  const shader *leaves_shader;
  const texture *leaf_tex;

public:
  random_tree(const shader *object_shader, const shader *leaves_shader,
              const texture *leaf_texture, double xpos, double ypos,
              double zpos);
  ~random_tree();
  virtual void render(const glm::mat4 *viewProjection, glm::vec3 viewPos,
                      const std::list<const light *> &lights,
                      glm::vec3 ambient) const;
};

inline random_tree::random_tree(const shader *object_shader,
                                const shader *leaves_shader,
                                const texture *leaf_texture, double xpos,
                                double ypos, double zpos)
    : object(object_shader, &tree, xpos, ypos, zpos),
      segment_count(glm::linearRand(MIN_SEGMENTS, MAX_SEGMENTS)),
      tip_y(glm::linearRand(MIN_TIP_Y, MAX_TIP_Y)),
      tree(segment_count, SEGMENT_HEIGHT,
           glm::linearRand(MIN_BARK_RADIUS, MAX_BARK_RADIUS), BARK_VARIANCE,
           tip_y),
      tex(TEXTURE_PATH("poplar.jpg")), norm(TEXTURE_PATH("poplar_normal.jpg")),
      leaves_shader(leaves_shader), leaf_tex(leaf_texture) {
  tree.init();
  add_texture(&tex, "texture0");
  add_texture(&norm, "normal0");
  for (uint8_t i = 0; i < 5; i++) {
    object *leaf =
        new object(leaves_shader, model_loader::get().get_cube(), xpos,
                   ypos + (segment_count * SEGMENT_HEIGHT) + tip_y / 2, zpos);
    leaf->add_texture(leaf_tex, "texture0");
    leaf->set_rotation(glm::ballRand((float)(2.f * M_PI)));
    leaf->set_scale(glm::linearRand(1.f, LEAF_MAX_SIZE),
                    glm::linearRand(1.f, LEAF_MAX_SIZE),
                    glm::linearRand(1.f, LEAF_MAX_SIZE));
    leaves.push_back(leaf);
  }
}

inline random_tree::~random_tree() {}

inline void random_tree::render(const glm::mat4 *viewProjection,
                                glm::vec3 viewPos,
                                const std::list<const light *> &lights,
                                glm::vec3 ambient) const {
  object::render(viewProjection, viewPos, lights, ambient);
  leaves_shader->use();
  leaves_shader->apply_uniform_mat4(*viewProjection, "viewProjection");
  leaves_shader->apply_uniform_vec3(ambient, "ambientLight");
  leaves_shader->apply_uniform_vec3(viewPos, "viewPos");
  leaves_shader->apply_uniform_scalar(1e10, "shininess");
  leaf_tex->set_active_texture(leaves_shader, 0, "texture0");
  for (object *leaf : leaves) {
    leaves_shader->apply_uniform_mat4(leaf->get_model_matrix(), "model");
    leaf->draw();
  }
}
