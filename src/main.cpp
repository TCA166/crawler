
#include <iostream>
#include <thread>

#include "scenes/game.hpp"
#include "scenes/radar.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW error: 0x%x, %s\n", error, description);
}

/*!
 @brief A function that creates a new renderer, and runs it
 @param target_scene The scene to render
 @param target_camera The camera to render the scene with
 @param width The width of the window
 @param height The height of the window
 @param name The name of the window
 @param mutex The mutex to lock the renderer with
*/
static void renderer_thread(scene *target_scene, camera *target_camera,
                            uint32_t width, uint32_t height, const char *name,
                            std::mutex *mutex, bool *should_close) {
  // renderer handles all the initialization
  renderer current_renderer(width, height, name, mutex, target_camera,
                            target_scene, should_close);

  // loop of the renderer
  current_renderer.run();
}

int main() {
  if (glfwInit() == GLFW_FALSE) {
    const char *desc;
    int code = glfwGetError(&desc);
    std::cerr << "GLFW error: " << code << ", " << desc << std::endl;
    return -1;
  }
#ifndef WASM
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
#endif
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  glfwSetErrorCallback(glfw_error_callback);

  std::srand(std::time(nullptr));

  std::list<boid *> boids;

  // a scene is a collection of objects
  game game_scene(boids);
  radar second_scene(boids);

  camera main_camera(glm::vec3(0.0f, 0.0f, 0.0f));

  std::mutex mutex;

  bool should_close = false;

#ifndef NO_THREADS
  std::thread loop_thread(&renderer_thread, &game_scene, &main_camera,
                          WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, &mutex,
                          &should_close);
  std::thread game_main_thread(&game::main, &game_scene, &main_camera,
                               &should_close);
  std::thread radar_thread(&renderer_thread, &second_scene, &main_camera,
                           RADAR_SIZE, RADAR_SIZE, "Radar", &mutex,
                           &should_close);
  /*
  std::thread radar_main_thread(&radar::main, &second_scene, &main_camera,
                                &should_close);*/
  // the game loop
  while (!should_close) {
    glfwWaitEvents();
  }
  game_main_thread.join();
  loop_thread.join();
  radar_thread.join();
  // radar_main_thread.join();
#else
  renderer_thread(&game_scene, &main_camera, WINDOW_WIDTH, WINDOW_HEIGHT,
                  WINDOW_NAME, &mutex);
#endif

  glfwTerminate();
  return 0;
}
