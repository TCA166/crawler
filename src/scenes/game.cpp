#include "game.hpp"
#include "../game_object.hpp"
#include "../engine/utils/model_loader.hpp"
#include <iostream>

const float camera_speed = 10.;

game::game(std::list<boid *> &boids)
    : scene(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.0)), mv_forward(false),
      mv_backward(false), mv_left(false), mv_right(false), mv_up(false),
      mv_down(false), rot_left(false), rot_right(false), xpos(0.0), ypos(0.0),
      boids(boids) {}

game::~game() {
  if (!initialized) {
    return;
  }
  delete this->textured_shader;
  delete this->skybox_shader;
  delete this->cube1;
  delete this->cube2;
  delete this->wall;
  delete this->floor;
  delete this->lght;
  delete this->sky;
  delete this->tex;
  delete this->norm;
  delete this->depth;
  delete this->view;
  delete this->simple_shader;
  delete this->tex2;
  for (auto &tri : boids) {
    delete tri;
  }
}

void game::init(camera *target_camera) {
  if (initialized) {
    return;
  }
  textured_shader =
      new shader(SHADER_PATH("textured.vert"), SHADER_PATH("textured.frag"));
  simple_shader = new shader(SHADER_PATH("textured.vert"),
                             SHADER_PATH("simple_textured.frag"));
  lght = new light(glm::vec3(0.0f, 8.0f, -6.0f), glm::vec3(0.0, 0.0, -1.0),
                   glm::vec3(0.9, 0.9, 0.9), 90.0f, 100.0f);
  this->add_light(lght);

  lght2 = new light(glm::vec3(15.0f, 8.0f, 25.0f), glm::vec3(0.0, 0.0, -1.0),
                    glm::vec3(0.2, 0.5, 0.5), 90.0f, 100.0f);
  this->add_light(lght2);
  //to swiatlo 3 nie dziala
  //lght3 = new light(glm::vec3(0.0, 3.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
  //                  glm::vec3(1.0, 0.0, 0.0), 2.0f, 2.0f);
  //this->add_light(lght3);
  tex = new texture(TEXTURE_PATH("spaceship.jpg"));
  tex2 = new texture(TEXTURE_PATH("diamond.png"));
  norm = new texture(TEXTURE_PATH("spaceship_normal.jpg"));
  tree_texture = new texture(TEXTURE_PATH("bark2.png"));
  tree_normal = new texture(TEXTURE_PATH("bark_normal.png"));
  //cliff_texture = new texture(TEXTURE_PATH("cliff_texture2.png"));

  //cube1 = new debug_cube(textured_shader, tex, norm, 0.0, 0.1, 0.0);
  //this->add_object(cube1);
  //cube2 = new debug_cube(textured_shader, tex, norm, 1.0, 0.1, 1.0);
  //this->add_object(cube2);
  //wall = new debug_wall(textured_shader, tex, norm, 0.0, 1.0, -5.0);
 // wall->set_scale(10.0, 3.0, 1.0);
  //this->add_object(wall);
 // floor = new debug_wall(textured_shader, tex, norm, 0.0, -0.5, 0.0);
  //floor->set_rotation(1.57, 0.0, 0.0);
  //floor->set_scale(10.0, 10.0, 1.0);
  //this->add_object(floor);
  depth = lght->get_view_map();
  view = new debug_wall(simple_shader, depth, norm, 0.0, 3.0, 0.0);
  this->add_object(view);
  model2 = new debug_model(textured_shader, MODEL_PATH("/bridge.obj"), tex, norm, 0.0, -7.0, 0.0);
  model2->set_scale(0.6, 0.2, 0.2);
  model2->set_rotation(0.0, 30.0, 0.0);
  this->add_object(model2);
  
std::vector<glm::vec3> tree_positions = {
      {15.0f, 8.0f, 23.0f},  {-18.0f, 10.0f, 19.0f},
      {0.0f, 12.0f, 17.0f},                          
      {25.0f, -5.0f, 10.0f},  {30.0f, 15.0f, -10.0f}, 
      {-25.0f, 7.0f, 15.0f}, {-30.0f, 20.0f, -5.0f}  
  };

for (size_t i = 0; i < tree_positions.size(); ++i) {
  const glm::vec3 &tree_pos = tree_positions[i];
  debug_model *tree = new debug_model(
      textured_shader, MODEL_PATH("/Lowpoly_tree_sample.obj"), tree_texture,
      tree_normal, tree_pos.x, tree_pos.y, tree_pos.z);
  tree->set_scale(0.7, 0.7, 0.7);
  this->add_object(tree);
  trees.push_back(tree);

  debug_model *cliff =
      new debug_model(textured_shader, MODEL_PATH("/island.obj"), tree_texture,
                      tree_normal, tree_pos.x, tree_pos.y - 2.0f, tree_pos.z);
  cliff->set_scale(0.7, 0.7, 0.7);
  this->add_object(cliff);
  cliffs.push_back(cliff); 
}

  for (int i = 0; i < 15; ++i) {
    float x = glm::linearRand(-10.0f, 10.0f);
    float y = glm::linearRand(1.0f, 5.0f);
    float z = glm::linearRand(-10.0f, 10.0f);
    boid *tri =
        new boid(textured_shader, MODEL_PATH("/ducky.obj"), x, y, z, tex2);
    boids.push_back(tri);
    this->add_object(tri);
  }

  skybox_shader =
      new shader(SHADER_PATH("skybox.vert"), SHADER_PATH("skybox.frag"));
  std::vector<std::string> paths = {
      TEXTURE_PATH("skybox/right.png"), TEXTURE_PATH("skybox/left.png"),
      TEXTURE_PATH("skybox/top.png"),   TEXTURE_PATH("skybox/bottom.png"),
      TEXTURE_PATH("skybox/front.png"), TEXTURE_PATH("skybox/back.png")};
  sky = new skybox(skybox_shader, paths);
  this->set_skybox(sky);
  target_camera->set_position(0.0, 1.0, 0.0);
  scene::init(target_camera);
}

void game::update(camera *target_camera, double delta_time,
                  double current_time) {

  glm::vec3 camera_position = target_camera->get_position();
  glm::vec3 camera_front = target_camera->get_front();
  glm::vec3 camera_right = target_camera->get_right();

  std::vector<glm::vec3> tree_positions = {
      {15.0f, 8.0f, 23.0f},  {-18.0f, 10.0f, 19.0f}, {0.0f, 12.0f, 17.0f},
      {25.0f, -5.0f, 10.0f}, {30.0f, 15.0f, -10.0f}, {-25.0f, 7.0f, 15.0f},
      {-30.0f, 20.0f, -5.0f}   
  };

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
    move = glm::normalize(move) * camera_speed * (float)delta_time;
    glm::vec3 collision_dist = move * (RENDER_MIN * 5e2f + 1.f);
    if (!check_line(camera_position, camera_position + collision_dist)) {
      target_camera->translate(move);
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
  

  for (size_t i = 0; i < trees.size(); ++i) {
    debug_model *tree = trees[i];
    float base_y = tree_positions[i].y;          
    float offset = sin(current_time + i) * 2.0f + 1.0f; 
    tree->set_position(tree_positions[i].x, base_y + offset,
                       tree_positions[i].z);
  }

  for (size_t i = 0; i < cliffs.size(); ++i) {
    debug_model *cliff = cliffs[i];
    float base_y = tree_positions[i].y - 2.0f;  
    float offset = sin(current_time + i) * 2.0f + 1.0f; 
    cliff->set_position(tree_positions[i].x, base_y + offset,
                        tree_positions[i].z);
  }

  glm::vec3 lght_pos = lght->get_position();
  lght_pos.x = sin(current_time) *
               4.; // no need for * delta_time because we sine current_time
  lght->set_position(lght_pos.x, lght_pos.y, lght_pos.z);

  
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
