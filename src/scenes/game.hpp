
#pragma once

#include "../engine/engine.hpp"
#include "../objects/boid.hpp"
#include "../objects/debug_cube.hpp"
#include "../objects/debug_wall.hpp"
#include "../objects/random_floor.hpp"
#include "../objects/tree.hpp"

/*!
 @brief The scene used for the game.
 @note This is where all the code for the game is written.
*/
class game : public scene {
private:
  bool mv_forward, mv_backward, mv_left, mv_right, mv_up, mv_down, rot_left,
      rot_right, shooting;
  double xpos, ypos;
  skybox *sky;
  light *lght;
  shader *textured_shader, *skybox_shader, *simple_shader, *debug_shader;
  std::list<boid *> &boids;
  bool is_shooting;
  glm::vec3 shoot_direction;
  random_floor *floor1;
  std::vector<boid_species *> species;
  std::vector<random_tree *> trees;
  texture *boid_tex, *boid_norm;

public:
  /*!
   @brief Initializes the main game scene
   @param boids The list of boids to use
  */
  game(std::list<boid *> &boids);
  ~game();
  /*!
   @brief Initialize the scene
   @param target_camera The camera that will be used to render the scene
  */
  void init(camera *target_camera);
  /*!
   @brief Performs a single game tick
   @param target_camera The camera used
   @param delta_time Time elapsed from last tick
   @param current_time the current time
  */
  void update(camera *target_camera, double delta_time, double current_time);
  /*!
   @brief Handle a scroll event
   @param xoffset The x offset of the scroll
   @param yoffset The y offset of the scroll
   @param target_camera The camera to scroll
  */
  void scroll_callback(double xoffset, double yoffset, camera &target_camera);
  /*!
   @brief Handle a key event
   @param key The key that was pressed
   @param scancode The scancode of the key
   @param action The action that was performed
   @param mods The mods that were pressed
   @param target_camera The camera to handle the key event
  */
  void key_callback(int key, int scancode, int action, int mods,
                    camera &target_camera);
  /*!
   @brief Handle a mouse event
   @param xpos The x position of the mouse
   @param ypos The y position of the mouse
   @param target_camera The camera to handle the mouse event
  */
  void mouse_callback(double xpos, double ypos, camera &target_camera);
};
