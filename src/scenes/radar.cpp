#include "radar.hpp"

static const float radar_radius = RADAR_SIZE / 20.f;

static const glm::vec2 radar_mid_point =
    glm::vec2(RADAR_SIZE / 2, RADAR_SIZE / 2);

radar::radar(std::list<boid *> &boids)
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), boids(boids) {

  radar_image.data = new unsigned char[RADAR_SIZE * RADAR_SIZE * 3];
  radar_image.width = RADAR_SIZE;
  radar_image.height = RADAR_SIZE;
  radar_image.nr_channels = 3;
}

void radar::draw_line(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t thickness) {
  int dx = abs(x - radar_mid_point.x);
  int dy = abs(y - radar_mid_point.y);
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
          radar_image.data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3] = r;
          radar_image.data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3 + 1] = g;
          radar_image.data[cx_offset * 3 + cy_offset * RADAR_SIZE * 3 + 2] = b;
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

void radar::draw_radar_cast(float angle, uint8_t r, uint8_t g, uint8_t b) {
  int x = RADAR_SIZE / 2 + (RADAR_SIZE / 2) * cos(angle);
  int y = RADAR_SIZE / 2 + (RADAR_SIZE / 2) * sin(angle);

  draw_line(x, y, r, g, b, 5);
}

void radar::init(camera *target_camera) {
  scene::init(target_camera);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void radar::update(camera *cam, double delta_time, double) {
  draw_radar_cast(angle - 0.1, 0, 0, 0);

  angle += delta_time * 4.f;
  if (angle > 2 * M_PI) {
    angle -= 2 * M_PI;
  }

  draw_radar_cast(angle, 0, 255, 0);

  glm::vec3 cam_pos = cam->get_position();
  // TODO rotate radar with camera
  glm::vec2 radar_center = glm::vec2(cam_pos.x, cam_pos.z);

  for (auto &tri : boids) {
    glm::vec3 pos = tri->get_position();
    glm::vec2 pos_radar =
        (radar_center - glm::vec2(pos.x, pos.z)) * radar_radius;
    pos_radar += radar_mid_point;
    // rotate the point so that the points rotate with cam_dir
    float distance = glm::distance(radar_mid_point, pos_radar);
    if (distance > RADAR_SIZE / 2) {
      continue;
    }
    radar_image.data[(int)pos_radar.x * 3 + (int)pos_radar.y * RADAR_SIZE * 3] =
        255;
  }
}

void radar::render(const camera &, uint16_t width, uint16_t height) {
  clear();

  texture radar_texture = texture(&radar_image);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  radar_texture.bind_to_fb();
  uint16_t out_size = std::min(width, height);
  glBlitFramebuffer(0, 0, RADAR_SIZE, RADAR_SIZE, 0, 0, out_size, out_size,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

radar::~radar() {}
