#pragma once

#include "../engine/engine.hpp"
#include "../engine/utils/noise.hpp"

/*!
 @brief The maximum value of the noise
*/
#define NOISE_MAX 1.5f
/*!
 @brief The amount of dillation to apply to the noise
*/
#define DILLATION 0.05f
/*!
 @brief The temperature of the noise (how random it is)
*/
#define NOISE_TEMP 10.f

/*!
 @brief A surface that is generated using Perlin noise
 @details The noise is generated using the noise function in noise.hpp, and then
 altered using the NOISE_MAX and DILLATION constants. Additionally the noise is
 randomly sampled based on the NOISE_TEMP constant. In OpenGL terms it is a
 simple triangle mesh, statically generated.
*/
class random_floor : public object {
private:
  glm::vec2 noise_shift;
  model floor;
  texture tex, norm;

public:
  /*!
   @brief Constructs a random floor object
   @param xpos The x position of the floor
   @param ypos The y position of the floor
   @param zpos The z position of the floor
   @param width The width of the floor
   @param height The height of the floor
   @param resolution The distance between each sample
  */
  random_floor(double xpos, double ypos, double zpos, uint32_t width,
               uint32_t height, float resolution);
  ~random_floor();
  /*!
   @brief Sample the noise at a given point
   @param x The x coordinate of the point
   @param y The y coordinate of the point
   @return The noise value at the given point
  */
  float sample_noise(float x, float y) const;
  /*!
   @brief Check if a point is below the floor
   @param point The point to check
   @return True if the point is below the floor, false otherwise
  */
  bool check_point_floor(glm::vec3 point) const;
  /*!
   @brief Check if a line intersects the floor
   @param a The start of the line
   @param b The end of the line
   @return True if the line intersects the floor, false otherwise
  */
  bool check_line_floor(glm::vec3 a, glm::vec3 b) const;
};

/*!
 @brief Sample the noise at a given point
 @param x The x coordinate of the point
 @param y The y coordinate of the point
 @return The noise value at the given point
 @note This is the local version of noise function, that is affected by the
  NOISE_MAX, DILLATION constants
*/
static inline float sample_noise(float x, float y) {
  return noise(x * DILLATION, y * DILLATION) * NOISE_MAX;
}

/*!
 @brief Generates the data for the floor, sampling the noise at each point
 @param width The width of the floor
 @param height The height of the floor
 @param resolution The distance between each sample
 @param noise_shift The amount to shift the noise by
 @return A vector of floats representing the points of the floor
*/
static inline std::vector<float> generate_data(uint32_t width, uint32_t height,
                                               float resolution,
                                               glm::vec2 noise_shift) {
  std::vector<float> points;
  for (uint32_t x = 0; x < width / resolution; x++) {
    for (uint32_t y = 0; y < height / resolution; y++) {
      points.push_back(x * resolution);
      points.push_back(sample_noise((x * resolution) + noise_shift.x,
                                    (y * resolution) + noise_shift.y));
      points.push_back(y * resolution);
      // texture coordinates
      points.push_back(x * resolution);
      points.push_back(y * resolution);
      // normals
      // https://stackoverflow.com/a/13983431/12520385 - simplified normal calc
      float height_l =
          sample_noise((x * resolution) - resolution + noise_shift.x,
                       (y * resolution) + noise_shift.y);
      float height_r =
          sample_noise((x * resolution) + resolution + noise_shift.x,
                       (y * resolution) + noise_shift.y);
      float height_d =
          sample_noise((x * resolution) + noise_shift.x,
                       (y * resolution) - resolution + noise_shift.y);
      float height_u =
          sample_noise((x * resolution) + noise_shift.x,
                       (y * resolution) + resolution + noise_shift.y);
      glm::vec3 normal = glm::normalize(
          glm::vec3(height_l - height_r, 2.0f, height_d - height_u));
      points.push_back(normal.x);
      points.push_back(normal.y);
      points.push_back(normal.z);
      // tangent
      glm::vec3 tangent = glm::normalize(
          glm::vec3(1.0, 0.0, (height_r - height_l) / (2.0f * resolution)));
      points.push_back(tangent.x);
      points.push_back(tangent.y);
      points.push_back(tangent.z);
      // bitangent
      glm::vec3 bitangent = glm::normalize(
          glm::cross(normal, tangent)); // cross product of normal and tangent
      points.push_back(bitangent.x);
      points.push_back(bitangent.y);
      points.push_back(bitangent.z);
    }
  }
  return points;
}

/*!
 @brief Generates the indices for the floor
 @param width The width of the floor
 @param height The height of the floor
 @return A vector of unsigned ints representing the indices of the floor
 @note This is a simple triangle mesh with 2 triangles per square
*/
static inline std::vector<unsigned int> generate_indices(uint32_t width,
                                                         uint32_t height) {
  std::vector<unsigned int> indices;
  for (uint32_t x = 0; x < width - 1; x++) {
    for (uint32_t y = 0; y < height - 1; y++) {
      indices.push_back(x * height + y);
      indices.push_back(x * height + y + 1);
      indices.push_back((x + 1) * height + y);
      indices.push_back((x + 1) * height + y);
      indices.push_back(x * height + y + 1);
      indices.push_back((x + 1) * height + y + 1);
    }
  }
  return indices;
}

inline random_floor::random_floor(double xpos, double ypos, double zpos,
                                  uint32_t width, uint32_t height,
                                  float resolution)
    : object(&floor, xpos, ypos, zpos),
      noise_shift(glm::linearRand(0.f, NOISE_TEMP),
                  glm::linearRand(0.f, NOISE_TEMP)),
      floor(generate_data(width, height, resolution, noise_shift),
            generate_indices(width / resolution, height / resolution),
            glm::vec3(width / resolution, NOISE_MAX, height / resolution),
            glm::vec3(0.0)),
      tex(TEXTURE_PATH("grass.jpg")), norm(TEXTURE_PATH("grass_normal.png")) {
  floor.init();
  this->add_texture(&tex, "texture0");
  this->add_texture(&norm, "normal0");
}

inline random_floor::~random_floor() {}

inline float random_floor::sample_noise(float x, float y) const {
  glm::vec3 position = get_position();
  return ::sample_noise(x + noise_shift.x - position.x,
                        y + noise_shift.y - position.z);
}

inline bool random_floor::check_point_floor(glm::vec3 point) const {
  return point.y < sample_noise(point.x, point.z);
}

inline bool random_floor::check_line_floor(glm::vec3 a, glm::vec3 b) const {
  if (!object::check_line(a, b)) {
    return false;
  }
  glm::vec3 direction = b - a;
  float distance = glm::length(direction);
  glm::vec3 step = direction / distance;
  for (float i = 0; i < distance; i += 0.1f) {
    if (check_point(a + step * i)) {
      return true;
    }
  }
  return false;
}
