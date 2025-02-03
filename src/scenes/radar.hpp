
#pragma once

#include "../engine/engine.hpp"
#include "../objects/boid.hpp"

#include <mutex>

#define RADAR_SIZE 500

/*!
 @brief A scene that shows a radar of the boids
*/
class radar : public scene {
private:
  std::list<boid *> &boids;
  GLuint PBO, texture_id, FBO;
  float angle, last_time;

  void draw_line(uint16_t x, uint16_t y, glm::vec3 color, uint8_t thickness,
                 uint8_t *data);
  void draw_radar_cast(float angle, glm::vec3 color, uint8_t *data);

public:
  /*!
   @brief Constructs the radar display
   @param boids The list of boids to show on the radar
  */
  radar(std::list<boid *> &boids);
  ~radar();
  /*!
   @brief Initialize the scene
   @param target_camera The camera that will be used to render the scene
  */
  void init(camera *target_camera);
  /*!
   @brief Render the scene
   @param target_camera The camera to render the scene with
   @param width The width of the viewport
   @param height The height of the viewport
  */
  void render(const camera &target_camera, uint16_t width, uint16_t height);
};
