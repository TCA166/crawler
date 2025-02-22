#include "game.hpp"
#include <iostream>

#define CAMERA_Y_OFFSET 1.0f
#define CAMERA_SPEED 10.0f

#define LIGHT_STRENGTH 0.9f
#define LIGHT_RANGE 30.0f
#define LIGHT_FOV glm::radians(70.f)

#define FLOOR_SIZE 100
#define TREE_COUNT 20
#define GRASS_COUNT 100

#define FLOCK_COUNT 5
// the radius from 0.0 to spawn boid flocks
#define SPAWN_RADIUS 10.0f
// the radius of the flock
#define FLOCK_RADIUS 5.0f
#define FLOCK_SIZE 6

#define MIN_FLOCK_Y 5.0f
#define MAX_FLOCK_Y 10.0f

#define MIN_FLOCK_SPEED 0.5f
#define MAX_FLOCK_SPEED 4.0f

#define MIN_FLOCK_SEP 2.0f
#define MAX_FLOCK_SEP 7.0f

#define MIN_FLOCK_COH 15.0f
#define MAX_FLOCK_COH 25.0f

#define LEAF_IMAGE_SIZE 128
#define COLOR_VARIANCE 25
#define LEAVES_PER_BRANCH 30
#define LEAF_SIZE 0.25f

#define SHOTGUN_FRONT_OFFSET 0.3f
#define LIGHT_FRONT_OFFSET 0.1f
#define RIGHT_OFFSET 0.15f

#define SPAWNING_RADIUS 3.0f

#define CAMERA_COLLISION_EPS (RENDER_MIN * 5e2f + 1.f)

game::game(std::list<boid *> &boids)
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), mv_forward(false),
      mv_backward(false), mv_left(false), mv_right(false), rot_left(false),
      rot_right(false), xpos(0.0), ypos(0.0), boids(boids) {}

game::~game() {
  if (!initialized) {
    return;
  }

  delete this->sky;

  delete this->lght;

  delete this->textured_shader;
  delete this->skybox_shader;

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
  leaf_shader =
      new shader(SHADER_PATH("leaves.vert"), SHADER_PATH("leaves.frag"), false);
  simple_textured_shader = new shader(SHADER_PATH("textured.vert"),
                                      SHADER_PATH("simple_textured.frag"));
  floor1 = new random_floor(FLOOR_SIZE / -2., 0.0, FLOOR_SIZE / -2., FLOOR_SIZE,
                            FLOOR_SIZE, 0.5);
  this->add_object(textured_shader, floor1);
  target_camera->set_position(
      glm::vec3(0.0, floor1->sample_noise(0.0, 0.0) + CAMERA_Y_OFFSET, 0.0));
  muzzle = new light(glm::vec3(), glm::vec3(1., 0.6, .24), glm::radians(180.f),
                     LIGHT_RANGE * 2.f, false);
  this->add_light(muzzle);
  lght = new light(target_camera->get_position(), glm::vec3(LIGHT_STRENGTH),
                   LIGHT_FOV, LIGHT_RANGE, true);
  this->add_light(lght);
  flash_image = new texture(TEXTURE_PATH("muzzle_flash.png"));
  flash_sprite = new object(model_loader::get().get_wall(), 0.f, 0.f, 0.f);
  flash_sprite->set_active(false);
  flash_sprite->add_texture(flash_image, "texture0");
  flash_sprite->set_scale(0.2);
  add_object(simple_textured_shader, flash_sprite);
  gun = new shotgun(muzzle, flash_sprite, 0.0f,
                    floor1->sample_noise(0.0, 0.0) + CAMERA_Y_OFFSET, 0.0f);
  this->add_object(textured_shader, gun);

  // flock spawning
  boid_tex = new texture(TEXTURE_PATH("diamond.png"));
  grasstex = new texture(TEXTURE_PATH("grass3.png"));
  boid_norm = new texture(TEXTURE_PATH("grass_normal.png"));
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
      boid *tri = new boid(boid_tex, boid_norm, pos.x, pos.y, pos.z, spec);
      boids.push_back(tri);
      this->add_object(textured_shader, tri);
    }
  }

  // tree spawning

  leaf_tex = create_random_leaf_texture(LEAF_IMAGE_SIZE, COLOR_VARIANCE);

  for (int x = FLOOR_SIZE / -2; x < FLOOR_SIZE / 2;
       x += FLOOR_SIZE / TREE_COUNT) {
    for (int z = FLOOR_SIZE / -2; z < FLOOR_SIZE / 2;
         z += FLOOR_SIZE / TREE_COUNT) {
      glm::vec2 pos = glm::vec2(x, z) + glm::circularRand(SPAWNING_RADIUS);
      random_tree *tree =
          new random_tree(pos.x, floor1->sample_noise(pos.x, pos.y), pos.y);
      this->add_object(textured_shader, tree);
      trees.push_back(tree);
      for (auto &pair : tree->get_leaves_points()) {
        glm::vec3 start_pos = pair.first + tree->get_position();
        glm::vec3 step = (pair.second - pair.first) / (float)LEAVES_PER_BRANCH;
        // plaster the trees along the branch
        for (uint8_t i = 1; i < LEAVES_PER_BRANCH; i++) {
          leaf_points.push_back((start_pos + step * (float)i) +
                                glm::ballRand(LEAF_SIZE));
        }
      }
    }
  }

  leaves_obj = new leaves(leaf_tex, leaf_points);
  this->add_object(leaf_shader, leaves_obj);
  leaves_obj->set_scale(LEAF_SIZE);
  // grass generation

  for (int x = FLOOR_SIZE / -2; x < FLOOR_SIZE / 2;
       x += FLOOR_SIZE / GRASS_COUNT) {
    for (int z = FLOOR_SIZE / -2; z < FLOOR_SIZE / 2;
         z += FLOOR_SIZE / GRASS_COUNT) {
      glm::vec2 pos = glm::vec2(x, z) + glm::circularRand(SPAWNING_RADIUS);
      float y = floor1->sample_noise(pos.x, pos.y) + 0.3;
      grass_points.push_back(glm::vec3(pos.x, y, pos.y));
    }
  }

  grass_obj = new grass(grasstex, grass_points);
  this->add_object(leaf_shader, grass_obj);

  skybox_shader =
      new shader(SHADER_PATH("skybox.vert"), SHADER_PATH("skybox.frag"));
  std::vector<std::string> paths = {
      TEXTURE_PATH("sky.png"), TEXTURE_PATH("sky.png"),
      TEXTURE_PATH("sky.png"), TEXTURE_PATH("sky.png"),
      TEXTURE_PATH("sky.png"), TEXTURE_PATH("sky.png")};
  sky = new skybox(paths);
  this->set_skybox(skybox_shader, sky);
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
      bool tree_collision = false;
      for (auto &tree : trees) {
        if (tree->check_line(camera_position, camera_position + move)) {
          tree_collision = true;
          break;
        }
      }
      if (!tree_collision) {
        target_camera->translate(move);
        camera_position = target_camera->get_position();
        camera_position.y =
            floor1->sample_noise(camera_position.x, camera_position.z) +
            CAMERA_Y_OFFSET;
        target_camera->set_position(camera_position);
      }
    }
  }
  if (rot_left) {
    target_camera->rotate(glm::vec3(0.0, 0.0, -delta_time));
  } else if (rot_right) {
    target_camera->rotate(glm::vec3(0.0, 0.0, delta_time));
  }

  for (auto &tri : boids) {
    tri->update((const std::list<const boid *> &)boids, this, delta_time);
  }

  gun->update(delta_time);
  if (shooting && gun->shoot()) {
    // Collision detection
    for (auto &tri : boids) {
      if (tri->check_line(camera_position,
                          camera_position + camera_front * 100.0f)) {
        this->remove_object(tri);
        tri->set_active(false);
        boids.remove(tri);
        break;
      }
    }

    shooting = false;
  }
  lght->set_position(camera_position + (camera_front * LIGHT_FRONT_OFFSET) -
                     (camera_right * RIGHT_OFFSET));
  lght->set_rotation(target_camera->get_rotation());
  gun->set_position(camera_position + (camera_front * SHOTGUN_FRONT_OFFSET) +
                    (camera_right * RIGHT_OFFSET) - glm::vec3(0.0, 0.3, 0.0));
  gun->look_at(camera_position + camera_front * 100.0f);
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
    if (pressed) {
      lght->set_active(!lght->is_active());
    }
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
    glm::vec3 rot(ypos - this->ypos, xpos - this->xpos, 0.0);
    target_camera.rotate(rot);
    this->xpos = xpos;
    this->ypos = ypos;
  }
}
