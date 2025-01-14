
#pragma once

#include "engine/engine.hpp"
#include "objects/boid.hpp"
#include "objects/debug_cube.hpp"
#include "objects/debug_wall.hpp"

/*!
 @brief The scene used for the game.
 @note This is where all the code for the game is written.
*/
class game : public scene {
private:
  double time_scale;
  double current_time, delta_time;
  bool mv_forward, mv_backward, mv_left, mv_right, mv_up, mv_down, rot_left,
      rot_right, shooting;
  double xpos, ypos;
  debug_cube *cube1, *cube2;
  debug_wall *floor, *wall, *view;
  skybox *sky;
  light *lght;
  shader *textured_shader, *skybox_shader, *simple_shader, *triangle_shader;
  texture *tex, *norm, *depth, *tex2;
  std::vector<boid *> boids;
  bool is_shooting;
  glm::vec3 shoot_direction;

public:
  game();
  ~game();
  /*!
   @brief Initialize the scene
  */
  virtual void init();
  /*!
   @brief Main function of the scene
   @param target_camera The camera that the scene is being rendered with
  */
  virtual void main(camera *target_camera);
  /*!
   @brief Handle a scroll event
   @param xoffset The x offset of the scroll
   @param yoffset The y offset of the scroll
   @param target_camera The camera to scroll
  */
  virtual void scroll_callback(double xoffset, double yoffset,
                               camera *target_camera);
  /*!
   @brief Handle a key event
   @param key The key that was pressed
   @param scancode The scancode of the key
   @param action The action that was performed
   @param mods The mods that were pressed
   @param target_camera The camera to handle the key event
  */
  virtual void key_callback(int key, int scancode, int action, int mods,
                            camera *target_camera);
  /*!
   @brief Handle a mouse event
   @param xpos The x position of the mouse
   @param ypos The y position of the mouse
   @param target_camera The camera to handle the mouse event
  */
  virtual void mouse_callback(double xpos, double ypos, camera *target_camera);
};
