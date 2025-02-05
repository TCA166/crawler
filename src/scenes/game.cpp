#include "game.hpp"
#include <iostream>

#define CAMERA_Y_OFFSET 1.0f
#define CAMERA_SPEED 10.0f

#define LIGHT_STRENGTH 0.5f
#define LIGHT_RANGE 15.0f
#define LIGHT_FOV glm::radians(70.f)
#define LIGHT_OFFSET 0.25f

#define FLOOR_SIZE 100

#define FLOCK_COUNT 10
// the radius from 0.0 to spawn boid flocks
#define SPAWN_RADIUS 10.0f
// the radius of the flock
#define FLOCK_RADIUS 5.0f
#define FLOCK_SIZE 15

#define MIN_FLOCK_Y 10.0f
#define MAX_FLOCK_Y 20.0f

#define MIN_FLOCK_SPEED 3.0f
#define MAX_FLOCK_SPEED 8.0f

#define MIN_FLOCK_SEP 5.0f
#define MAX_FLOCK_SEP 15.0f

#define MIN_FLOCK_COH 15.0f
#define MAX_FLOCK_COH 25.0f

#define CAMERA_COLLISION_EPS (RENDER_MIN * 5e2f + 1.f)

game::game(std::list<boid *> &boids)
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), mv_forward(false),
      mv_backward(false), mv_left(false), mv_right(false), mv_up(false),
      mv_down(false), rot_left(false), rot_right(false), xpos(0.0), ypos(0.0),
      boids(boids) {}

game::~game() {
  if (!initialized) {
    return;
  }
  delete this->cube1;
  delete this->cube2;

  delete this->wall;
  delete this->view;

  delete this->sky;

  delete this->lght;

  delete this->textured_shader;
  delete this->skybox_shader;
  delete this->simple_shader;

  delete this->norm;
  delete this->depth;
  delete this->tex;

  for (auto &tri : boids) {
    delete tri;
  }

  delete this->floor1;
}

void game::init(camera *target_camera) {
  if (initialized) {
    return;
  }
  textured_shader =
      new shader(SHADER_PATH("textured.vert"), SHADER_PATH("textured.frag"));
  simple_shader = new shader(SHADER_PATH("textured.vert"),
                             SHADER_PATH("simple_textured.frag"));
  floor1 = new random_floor(textured_shader, FLOOR_SIZE / -2., 0.0,
                            FLOOR_SIZE / -2., FLOOR_SIZE, FLOOR_SIZE, 0.1);
  this->add_object(floor1);
  cube1 = new debug_cube(textured_shader, 0.0,
                         0.1 + floor1->sample_noise(0., 0.), 0.0);
  this->add_object(cube1);
  this->add_collider(cube1);
  cube2 = new debug_cube(textured_shader, 1.0,
                         0.1 + floor1->sample_noise(1., 1.), 1.0);
  this->add_object(cube2);
  this->add_collider(cube2);
  norm = new texture(TEXTURE_PATH("spaceship_normal.jpg"));
  tex = new texture(TEXTURE_PATH("spaceship.jpg"));
  wall = new debug_wall(textured_shader, tex, norm, 0.0, 1.0, -5.0);
  wall->set_scale(10.0, 3.0, 1.0);
  this->add_object(wall);
  this->add_collider(wall);
  target_camera->set_position(
      0.0, floor1->sample_noise(0.0, 0.0) + CAMERA_Y_OFFSET, 0.0);
  lght = new light(target_camera->get_position() +
                       glm::vec3(-LIGHT_OFFSET, 0.f, LIGHT_OFFSET),
                   target_camera->get_front(), glm::vec3(LIGHT_STRENGTH),
                   LIGHT_FOV, LIGHT_RANGE);
  this->add_light(lght);
  depth = lght->get_view_map();
  view = new debug_wall(simple_shader, depth, norm, 0.0, 3.0, 0.0);
  this->add_object(view);
  // flock spawning
  for (uint8_t flock = 0; flock < FLOCK_COUNT; flock++) {
    boid_species *spec = new boid_species();

    spec->id = (uint32_t)flock;
    spec->pref_y = glm::linearRand(MIN_FLOCK_Y, MAX_FLOCK_Y);
    spec->max_speed = glm::linearRand(MIN_FLOCK_SPEED, MAX_FLOCK_SPEED);
    spec->ali_dist = 2.0f; // i see no reason to change this
    // 10 was default
    spec->sep_dist = glm::linearRand(MIN_FLOCK_SEP, MAX_FLOCK_SEP);
    // 20 was default
    spec->coh_dist = glm::linearRand(MIN_FLOCK_COH, MAX_FLOCK_COH);

    species.push_back(spec);
    glm::vec3 center =
        glm::vec3(glm::linearRand(-SPAWN_RADIUS, SPAWN_RADIUS), spec->pref_y,
                  glm::linearRand(-SPAWN_RADIUS, SPAWN_RADIUS));
    for (int i = 0; i < FLOCK_SIZE; ++i) {
      glm::vec3 pos = center + glm::ballRand(FLOCK_RADIUS);
      boid *tri = new boid(textured_shader, pos.x, pos.y, pos.z, spec);
      boids.push_back(tri);
      this->add_object(tri);
    }
  }

  skybox_shader =
      new shader(SHADER_PATH("skybox.vert"), SHADER_PATH("skybox.frag"));
  std::vector<std::string> paths = {
      TEXTURE_PATH("skybox/right.png"), TEXTURE_PATH("skybox/left.png"),
      TEXTURE_PATH("skybox/top.png"),   TEXTURE_PATH("skybox/bottom.png"),
      TEXTURE_PATH("skybox/front.png"), TEXTURE_PATH("skybox/back.png")};
  sky = new skybox(skybox_shader, paths);
  this->set_skybox(sky);
  scene::init(target_camera);
}

void game::update(camera *target_camera, double delta_time, double) {

  glm::vec3 camera_position = target_camera->get_position();
  glm::vec3 camera_front = target_camera->get_front();
  glm::vec3 camera_right = target_camera->get_right();

  glm::vec3 move = glm::vec3(0.0);
  if (mv_forward) {
    move += camera_front;
  } else if (mv_backward) {
    move -= camera_front;
  }
  if (mv_left) {
    move -= camera_right;
  } else if (mv_right) {
    move += camera_right;
  }
  if (move.x != 0. || move.z != 0) {
    move.y = 0;
    move = glm::normalize(move) * CAMERA_SPEED * (float)delta_time;
    glm::vec3 collision_dist = move * CAMERA_COLLISION_EPS;
    if (!check_line(camera_position, camera_position + collision_dist)) {
      target_camera->translate(move);
      camera_position = target_camera->get_position();
      target_camera->set_position(
          camera_position.x,
          floor1->sample_noise(camera_position.x, camera_position.z) +
              CAMERA_Y_OFFSET,
          camera_position.z);
    }
  }
  if (rot_left) {
    target_camera->rotate(0.0, 0.0, -delta_time);
  } else if (rot_right) {
    target_camera->rotate(0.0, 0.0, delta_time);
  }

  for (auto &tri : boids) {
    tri->update((const std::list<const boid *> &)boids, this, delta_time);
  }

  if (shooting) {
    glm::vec3 shoot_position = camera_position + camera_front * 10.0f;

    // Collision detection
    for (auto &tri : boids) {
      if (tri->check_line(camera_position, shoot_position)) {
        this->remove_object(tri);
        delete tri;
        boids.remove(tri);
        break;
      }
    }

    shooting = false;
  }
  glm::vec3 light_position =
      camera_position +
      (camera_front + glm::vec3(-LIGHT_OFFSET, 0.f, LIGHT_OFFSET));
  lght->set_position(light_position.x, light_position.y, light_position.z);
  lght->set_direction(light_position + camera_front);
}

void game::scroll_callback(double, double yoffset, camera &target_camera) {
  target_camera.zoom(yoffset);
}

void game::key_callback(int key, int, int action, int, camera &) {
  if (action == GLFW_REPEAT) {
    return;
  }
  bool pressed = action == GLFW_PRESS;
  switch (key) {
  case GLFW_KEY_W:
    mv_forward = pressed;
    break;
  case GLFW_KEY_S:
    mv_backward = pressed;
    break;
  case GLFW_KEY_A:
    mv_left = pressed;
    break;
  case GLFW_KEY_D:
    mv_right = pressed;
    break;
  case GLFW_KEY_SPACE:
    mv_up = pressed;
    break;
  case GLFW_KEY_LEFT_SHIFT:
    mv_down = pressed;
    break;
  case GLFW_KEY_Q:
    rot_left = pressed;
    break;
  case GLFW_KEY_E:
    rot_right = pressed;
    break;
  case GLFW_KEY_F: // Key for shooting
    shooting = pressed;
    break;
  }
}

void game::mouse_callback(double xpos, double ypos, camera &target_camera) {
  if (xpos != this->xpos || ypos != this->ypos) {
    target_camera.rotate(xpos - this->xpos, ypos - this->ypos, 0.0);
    this->xpos = xpos;
    this->ypos = ypos;
  }
}
