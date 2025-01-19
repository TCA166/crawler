
#pragma once

#include "../abc/collider.hpp"
#include "../renderable/object.hpp"
#include "../renderable/skybox.hpp"

#include <list>

/*!
 @brief Scene class to handle rendering of objects.
 */
class scene : public collider {
protected:
  /*!
   @brief Whether the scene has had its init function called
  */
  bool initialized = false;

private:
  std::list<object *> objects;
  std::list<light *> lights;
  glm::vec3 ambient_light;
  glm::vec3 background_color;
  bool should_close = false;
  skybox *sky;
  const shader *light_pass_shader;
  double current_time, delta_time;

public:
  /*!
   @brief Constructs a scene
   @param ambient_light The ambient light of the scene
   @param background_color The background color of the scene
  */
  scene(glm::vec3 ambient_light, glm::vec3 background_color);
  ~scene();
  /*!
   @brief Set the skybox of the scene
   @param sky The skybox to set
  */
  void set_skybox(skybox *sky);
  /*!
   @brief Add an object to the scene
   @param obj The object to add
  */
  void add_object(object *obj);
  /*!
   @brief Remove an object from the scene
   @param obj The object to remove
  */
  void remove_object(const object *obj);
  /*!
   @brief Add a light to the scene
   @param light The light to add
  */
  void add_light(light *light);
  /*!
   @brief Remove a light from the scene
   @param light The light to remove
  */
  void remove_light(const light *light);
  /*!
   @brief Initialize the scene
   @param target_camera the camera that will be used in the scene
  */
  virtual void init(camera &target_camera);
  /*!
   @brief Render the scene
   @param target_camera The camera to render the scene with
   @param aspect_ratio The aspect ratio of the window
  */
  void render(const camera &target_camera, float aspect_ratio);
  /*!
   @brief Perform the shadow pass
   @warning May modify the viewport
  */
  void shadow_pass() const;
  /*!
   @brief Main function of the scene
   @param target_camera The camera that the scene is being rendered with
  */
  void main(camera &target_camera);
  /*!
   @brief Performs a scene tick
   @param target_camera The camera in the scene
   @param delta_time The time elapsed from last tick
   @param current_time The current time
  */
  virtual void update(camera &target_camera, double delta_time,
                      double current_time);
  /*!
   @brief Handle a scroll event
   @param xoffset The x offset of the scroll
   @param yoffset The y offset of the scroll
   @param target_camera The camera to scroll
  */
  virtual void scroll_callback(double xoffset, double yoffset,
                               camera &target_camera);
  /*!
   @brief Handle a key event
   @param key The key that was pressed
   @param scancode The scancode of the key
   @param action The action that was performed
   @param mods The mods that were pressed
   @param target_camera The camera to handle the key event
  */
  virtual void key_callback(int key, int scancode, int action, int mods,
                            camera &target_camera);
  /*!
   @brief Handle a mouse button event
   @param button The button that was pressed
   @param action The action that was performed
   @param mods The mods that were pressed
   @param target_camera The camera to handle the mouse button event
  */
  virtual void mouse_button_callback(int button, int action, int mods,
                                     camera &target_camera);
  /*!
   @brief Handle a mouse event
   @param xpos The x position of the mouse
   @param ypos The y position of the mouse
   @param target_camera The camera to handle the mouse event
  */
  virtual void mouse_callback(double xpos, double ypos, camera &target_camera);
  /*!
   @brief Handle a close event
  */
  void close_callback();
  /*!
   @brief Get the should close value
   @return The should close value
  */
  bool get_should_close() const;
  /*!
   @brief Check if a point is inside the collider
   @param point The point to check
   @return True if the point is inside the collider
  */
  bool check_point(glm::vec3 point) const;
  /*!
   @brief Check if a line intersects the collider
   @param a The start of the line
   @param b The end of the line
   @return True if the line intersects the collider
  */
  bool check_line(glm::vec3 a, glm::vec3 b) const;
};
