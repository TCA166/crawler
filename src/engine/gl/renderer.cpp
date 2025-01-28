
#include "renderer.hpp"

#include <iostream>
#include <stdexcept>
#include <thread>

#include "../utils/model_loader.hpp"

static const glm::vec3 loading_color = glm::vec3(038.0f, 206.0f, 0.0f);

static std::map<GLenum, const GLchar *> gl_source_name_map = {
    {GL_DEBUG_SOURCE_API, "API"},
    {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
    {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
    {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
    {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
    {GL_DEBUG_SOURCE_OTHER, "OTHER"}};

static std::map<GLenum, const GLchar *> gl_severity_name_map = {
    {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
    {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
    {GL_DEBUG_SEVERITY_LOW, "LOW"},
    {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}};

static std::map<GLenum, const GLchar *> gl_type_name_map = {
    {GL_DEBUG_TYPE_ERROR, "ERROR"},
    {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
    {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
    {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
    {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
    {GL_DEBUG_TYPE_MARKER, "MARKER"},
    {GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
    {GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
    {GL_DEBUG_TYPE_OTHER, "OTHER"}};

static void APIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id,
                                         GLenum severity, GLsizei, // length
                                         const GLchar *message,
                                         const void *) { // userParam
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    return; // ignore notifications
  }
  fprintf(stderr, "GL: %s %s, %s: ID: %d, %s\n", gl_type_name_map[type],
          gl_severity_name_map[severity], gl_source_name_map[source], id,
          message);
}

/*!
 @brief Global framebuffer size callback
 @details This function is called when the window is resized, it expects a
 renderer instance as the user pointer
 @param window The window that was resized
 @param width The new width of the window
 @param height The new height of the window
*/
static void global_framebuffer_size_callback(GLFWwindow *window, int width,
                                             int height) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->resize(width, height);
}

/*!
 @brief Global key callback
 @details This function is called when a key is pressed, it expects a renderer
 instance as the user pointer
 @param window The window that was resized
 @param key The key that was pressed
 @param scancode The scancode of the key
 @param action The action that was performed
 @param mods The mods that were pressed
*/
static void global_key_callback(GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->key_callback(key, scancode, action, mods);
}

/*!
 @brief Global mouse button callback
 @details This function is called when a mouse button is pressed, it expects a
 renderer instance as the user pointer
 @param window The window that was resized
 @param button The button that was pressed
 @param action The action that was performed
 @param mods The mods that were pressed
*/
static void global_mouse_button_callback(GLFWwindow *window, int button,
                                         int action, int mods) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->mouse_button_callback(button, action, mods);
}

/*!
 @brief Global scroll callback
 @details This function is called when the mouse is scrolled, it expects a
 renderer instance as the user pointer
 @param window The window that was resized
 @param xoffset The x offset of the scroll
 @param yoffset The y offset of the scroll
*/
static void global_scroll_callback(GLFWwindow *window, double xoffset,
                                   double yoffset) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->scroll_callback(xoffset, yoffset);
}

/*!
 @brief Global mouse callback
 @details This function is called when the mouse is moved, it expects a renderer
 instance as the user pointer
 @param window The window that was resized
 @param xpos The x position of the mouse
 @param ypos The y position of the mouse
*/
static void global_mouse_callback(GLFWwindow *window, double xpos,
                                  double ypos) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->mouse_callback(xpos, ypos);
}

/*!
 @brief Global window close callback
 @details This function is called when the window is closed, it expects a
 renderer instance as the user pointer
 @param window The window that should be closed
*/
static void global_window_close_callback(GLFWwindow *window) {
  renderer *instance =
      static_cast<renderer *>(glfwGetWindowUserPointer(window));
  instance->close_callback();
}

renderer::renderer(int width, int height, const char *name, std::mutex *mutex,
                   camera *render_camera, scene *target_scene,
                   bool *should_close)
    : renderer(width, height, name, mutex, render_camera, target_scene,
               should_close, NULL) {}

renderer::renderer(int width, int height, const char *name, std::mutex *mutex,
                   camera *render_camera, scene *target_scene,
                   bool *should_close, GLFWwindow *parent_window)
    : width(width), height(height), focused(false), render_mutex(mutex),
      should_close(should_close) {
  {
    std::lock_guard<std::mutex> lock(*mutex);
    this->window = glfwCreateWindow(width, height, name, NULL, parent_window);
    if (window == NULL) {
      const char *desc;
      glfwGetError(&desc);
      throw std::runtime_error(desc);
    }
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
      throw std::runtime_error("Failed to initialize GLEW");
    }
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifndef WASM
    glfwSwapBuffers(window);
#endif

    glEnable(GL_DEBUG_OUTPUT);
#ifndef WASM
    glDebugMessageCallback(gl_message_callback, NULL);
#endif

    glfwMakeContextCurrent(NULL);
  } // exiting the scope will unlock the mutex

  glfwSetFramebufferSizeCallback(window, global_framebuffer_size_callback);
  glfwSetKeyCallback(window, global_key_callback);
  glfwSetMouseButtonCallback(window, global_mouse_button_callback);
  glfwSetScrollCallback(window, global_scroll_callback);
  glfwSetCursorPosCallback(window, global_mouse_callback);
#ifndef WASM
  glfwSetWindowCloseCallback(window, global_window_close_callback);
#endif

  this->target_camera = render_camera;
  change_scene(target_scene);
}

renderer::~renderer() { glfwDestroyWindow(window); }

renderer renderer::clone(const char *name, scene *new_scene) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  renderer new_renderer =
      renderer(width, height, name, render_mutex, target_camera, new_scene,
               should_close, window);
  return new_renderer;
}

void renderer::key_callback(int key, int scancode, int action, int mods) {
  if (!focused || target_scene == nullptr) {
    return;
  }
  if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
    this->unfocus();
  }
  this->target_scene->key_callback(key, scancode, action, mods, *target_camera);
}

void renderer::mouse_button_callback(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !focused) {
    this->focus();
  }
  if (focused && target_scene != nullptr) {
    this->target_scene->mouse_button_callback(button, action, mods,
                                              *target_camera);
  }
}

void renderer::mouse_callback(double xpos, double ypos) {
  if (!focused || target_scene == nullptr) {
    return;
  }
  ypos = -ypos;
  this->target_scene->mouse_callback(xpos, ypos, *target_camera);
}

void renderer::scroll_callback(double xoffset, double yoffset) {
  if (focused && target_scene != nullptr) {
    this->target_scene->scroll_callback(xoffset, yoffset, *target_camera);
  }
}

void renderer::run() {
#ifdef NO_THREADS
  double current_time = glfwGetTime();
#endif
  while (!glfwWindowShouldClose(window) && !*should_close) {
    // render_mutex->lock();            // we wait for our turn to render
    glfwMakeContextCurrent(window);  // tell openGL we are outputting to this
    target_scene->shadow_pass();     // create shadow maps
    glViewport(0, 0, width, height); // swap back to our resolution
    // render the scene
    target_scene->render(*target_camera, width, height);
#ifndef WASM
    glfwSwapBuffers(window);
#endif
#ifdef _WIN32
#warning "Kompilowane na zjebanym systemie operacyjnym!"
    glfwPollEvents();
#endif
    glfwMakeContextCurrent(NULL);
    // show the rendered scene
#ifdef NO_THREADS
    double new_time = glfwGetTime();
    double delta_time = new_time - current_time;
    current_time = new_time;
    target_scene->update(target_camera, delta_time, current_time);
#endif
  }
}

void renderer::resize(int width, int height) {
  this->width = width;
  this->height = height;
}

void renderer::change_scene(scene *new_scene) {
  render_mutex->lock();
  glfwMakeContextCurrent(window);
  show_loading();
  // we need to make sure something is polling events, else the OS will think
  // the program is unresponsive
  new_scene->init(target_camera);
  model_loader::get().init();
  render_mutex->unlock();
  target_scene = new_scene;
}

void renderer::focus() {
  focused = true;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void renderer::unfocus() {
  focused = false;
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void renderer::close_callback() {
  glfwSetWindowShouldClose(window, GLFW_TRUE);
  *should_close = true;
}

void renderer::show_loading() {
  glClearColor(loading_color.r, loading_color.g, loading_color.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifndef WASM
  glfwSwapBuffers(window);
#endif
}
