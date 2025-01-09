
#include <iostream>
#include <thread>

#include "game.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

int main() {
  if (glfwInit() == GLFW_FALSE) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // a scene is a collection of objects
  game game_scene = game();

  camera main_camera = camera(glm::vec3(0.0f, 0.0f, 0.0f));

  std::mutex mutex;

  // renderer handles all the initialization
  renderer current_renderer =
      renderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, &mutex, &main_camera);

  current_renderer.change_scene(static_cast<scene *>(&game_scene));

  std::thread loop_thread = std::thread(&game::main, &game_scene, &main_camera);
  // renderer loop
  current_renderer.run();
  loop_thread.join();

  glfwTerminate();
  return 0;
}
