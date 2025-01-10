
#include <iostream>
#include <thread>

#include "game.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW error: 0x%x, %s\n", error, description);
}

static void renderer_thread(scene *target_scene, camera *target_camera,
                            std::mutex *mutex) {

  // renderer handles all the initialization
  renderer current_renderer =
      renderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, mutex, target_camera);

  current_renderer.change_scene(static_cast<scene *>(target_scene));

  // loop of the renderer
  current_renderer.run();
}

int main() {
  if (glfwInit() == GLFW_FALSE) {
    const char *desc;
    int code = glfwGetError(&desc);
    glfw_error_callback(code, desc);
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  glfwSetErrorCallback(glfw_error_callback);

  // a scene is a collection of objects
  game game_scene = game();

  camera main_camera = camera(glm::vec3(0.0f, 0.0f, 0.0f));

  std::mutex mutex;

  std::thread loop_thread =
      std::thread(&renderer_thread, &game_scene, &main_camera, &mutex);
  // the game loop
  game_scene.main(&main_camera);
  loop_thread.join();

  glfwTerminate();
  return 0;
}
