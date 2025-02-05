#include "radar.hpp"
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SCALE 30.f

static const float radar_radius = RADAR_SIZE / SCALE;

static const glm::vec2 radar_mid_point =
    glm::vec2(RADAR_SIZE / 2, RADAR_SIZE / 2);

radar::radar(std::list<boid *> &boids)
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), boids(boids),
      last_time(glfwGetTime()) {}

void radar::draw_line(uint16_t x, uint16_t y, glm::vec3 color,
                      uint8_t thickness, uint8_t *data) {
  int dx = std::abs(x - radar_mid_point.x);
  int dy = std::abs(y - radar_mid_point.y);
  int sx = radar_mid_point.x < x ? 1 : -1;
  int sy = radar_mid_point.y < y ? 1 : -1;
  int err = dx - dy;

  for (int i = -thickness / 2; i <= thickness / 2; ++i) {
    for (int j = -thickness / 2; j <= thickness / 2; ++j) {
      int cx_offset = radar_mid_point.x + i;
      int cy_offset = radar_mid_point.y + j;
      while (true) {
        if (cx_offset >= 0 && cx_offset < RADAR_SIZE && cy_offset >= 0 &&
            cy_offset < RADAR_SIZE) {
          data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3] = color.r;
          data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3 + 1] = color.g;
          data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3 + 2] = color.b;
        }

        if (cx_offset == x + i && cy_offset == y + j)
          break;
        int e2 = 2 * err;
        if (e2 > -dy) {
          err -= dy;
          cx_offset += sx;
        }
        if (e2 < dx) {
          err += dx;
          cy_offset += sy;
        }
      }
    }
  }
}

void radar::draw_radar_cast(float angle, glm::vec3 color, uint8_t *data) {
  int x = RADAR_SIZE / 2 + (RADAR_SIZE / 2) * cos(angle);
  int y = RADAR_SIZE / 2 + (RADAR_SIZE / 2) * sin(angle);

  draw_line(x, y, color, 8, data);
}

void radar::init(camera *target_camera) {
  scene::init(target_camera);

  glGenBuffers(1, &PBO);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, RADAR_SIZE * RADAR_SIZE * 3, NULL,
               GL_STREAM_DRAW);

  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer is not complete");
  }

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RADAR_SIZE, RADAR_SIZE, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void radar::render(const camera &cam, uint16_t width, uint16_t height) {
  clear();

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  uint8_t *ptr = (uint8_t *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if (ptr == NULL) {
    throw std::runtime_error("Failed to map PBO");
  }
  draw_radar_cast(angle, glm::vec3(0), ptr);
  angle += (glfwGetTime() - last_time) * 4.f;
  if (angle > 2 * M_PI) {
    angle -= 2 * M_PI;
  }
  draw_radar_cast(angle, glm::vec3(0, 255, 0), ptr);

  glm::vec3 cam_pos = cam.get_position();
  float cam_yaw = glm::radians(cam.get_rotation().y + 90);
  glm::vec2 radar_center = glm::vec2(cam_pos.x, cam_pos.z);

  float cos_yaw = cos(cam_yaw);
  float sin_yaw = sin(cam_yaw);
  glm::mat2 rotation_matrix = glm::mat2(cos_yaw, -sin_yaw, sin_yaw, cos_yaw);

  for (auto tri : boids) {
    glm::vec3 pos = tri->get_position();
    glm::vec2 pos_radar =
        (rotation_matrix *
         ((radar_center - glm::vec2(pos.x, pos.z)) * radar_radius)) +
        radar_mid_point;
    float distance = glm::distance(radar_mid_point, pos_radar);
    if (distance > RADAR_SIZE / 2 || glm::isnan(pos_radar.x) ||
        glm::isnan(pos_radar.y)) {
      continue;
    }
    ptr[(int)pos_radar.x * 3 + (int)pos_radar.y * RADAR_SIZE * 3] = 255;
  }
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, RADAR_SIZE, RADAR_SIZE, GL_RGB,
                  GL_UNSIGNED_BYTE, NULL);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D, texture_id, 0);
  uint16_t out_size = std::min(width, height);
  glBlitFramebuffer(0, 0, RADAR_SIZE, RADAR_SIZE, 0, 0, out_size, out_size,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
  last_time = glfwGetTime();
}

radar::~radar() {}
