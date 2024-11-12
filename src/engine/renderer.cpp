
#include "renderer.hpp"

#include <stdexcept>
#include <iostream>

/*!
 @brief Global framebuffer size callback
 @details This function is called when the window is resized, it expects a renderer instance as the user pointer
 @param window The window that was resized
 @param width The new width of the window
 @param height The new height of the window
*/
static void global_framebuffer_size_callback(GLFWwindow* window, int width, int height){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
	instance->resize(width, height);
}

/*!
 @brief Global key callback
 @details This function is called when a key is pressed, it expects a renderer instance as the user pointer
 @param window The window that was resized
 @param key The key that was pressed
 @param scancode The scancode of the key
 @param action The action that was performed
 @param mods The mods that were pressed
*/
static void global_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->key_callback(key, scancode, action, mods);
}

/*!
 @brief Global mouse button callback
 @details This function is called when a mouse button is pressed, it expects a renderer instance as the user pointer
 @param window The window that was resized
 @param button The button that was pressed
 @param action The action that was performed
 @param mods The mods that were pressed
*/
static void global_mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->mouse_button_callback(button, action, mods);
}

/*!
 @brief Global scroll callback
 @details This function is called when the mouse is scrolled, it expects a renderer instance as the user pointer
 @param window The window that was resized
 @param xoffset The x offset of the scroll
 @param yoffset The y offset of the scroll
*/
static void global_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->scroll_callback(xoffset, yoffset);
}

/*!
 @brief Global mouse callback
 @details This function is called when the mouse is moved, it expects a renderer instance as the user pointer
 @param window The window that was resized
 @param xpos The x position of the mouse
 @param ypos The y position of the mouse
*/
static void global_mouse_callback(GLFWwindow* window, double xpos, double ypos){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->mouse_callback(xpos, ypos);
}

/*!
 @brief Global window close callback
 @details This function is called when the window is closed, it expects a renderer instance as the user pointer
 @param window The window that should be closed
*/
static void global_window_close_callback(GLFWwindow* window){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->close_callback();
}

renderer::renderer(int width, int height, const char* name, sem_t* semaphore, camera* render_camera) : renderer(width, height, name, semaphore, render_camera, NULL) {}

renderer::renderer(int width, int height, const char* name, sem_t* semaphore, camera* render_camera, GLFWwindow* parent_window) {
    this->width = width;
    this->height = height;
    this->window = glfwCreateWindow(width, height, name, NULL, parent_window);
    if (window == NULL) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(window, this);
    sem_wait(semaphore);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("Failed to initialize GLEW");
    }
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);

    sem_post(semaphore);

    glfwSetFramebufferSizeCallback(window, global_framebuffer_size_callback);
    glfwSetKeyCallback(window, global_key_callback);
    glfwSetMouseButtonCallback(window, global_mouse_button_callback);
    glfwSetScrollCallback(window, global_scroll_callback);
    glfwSetCursorPosCallback(window, global_mouse_callback);
    glfwSetWindowCloseCallback(window, global_window_close_callback);
    
    this->focused = false;
    this->semaphore = semaphore;
    this->target_camera = render_camera;
}

renderer::~renderer() {
    glfwDestroyWindow(window);
}

renderer renderer::clone(const char* name){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    renderer new_renderer = renderer(width, height, name, semaphore, target_camera, window);
    new_renderer.change_scene(target_scene);
    return new_renderer;
}

void renderer::key_callback(int key, int scancode, int action, int mods){
    if(!focused){
        return;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE){
        this->unfocus();
    }
    this->target_scene->key_callback(key, scancode, action, mods, target_camera);
}

void renderer::mouse_button_callback(int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !focused){
        this->focus();
    }
    if(focused){
        this->target_scene->mouse_button_callback(button, action, mods, target_camera);
    }
}

void renderer::mouse_callback(double xpos, double ypos){
    if(!focused){
        return;
    }
    ypos = -ypos;
    this->target_scene->mouse_callback(xpos, ypos, target_camera);
}

void renderer::scroll_callback(double xoffset, double yoffset){
    if(focused){
        this->target_scene->scroll_callback(xoffset, yoffset, target_camera);
    }
}

void renderer::run() {
    if(target_scene == NULL){
        throw std::runtime_error("No scene to render");
    }
    while(!glfwWindowShouldClose(window) && !target_scene->get_should_close()){
        sem_wait(semaphore);
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
        target_scene->render(target_camera, width / float(height));
        sem_post(semaphore);
        glfwSwapBuffers(window);
    }
}

void renderer::resize(int width, int height){
    this->width = width;
    this->height = height;
}

void renderer::change_scene(scene* new_scene){
    sem_wait(semaphore);
    glfwMakeContextCurrent(window);
    new_scene->init();
    sem_post(semaphore);
    target_scene = new_scene;
}

void renderer::focus(){
    focused = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void renderer::unfocus(){
    focused = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void renderer::close_callback(){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    target_scene->close_callback();
}
