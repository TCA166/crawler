
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
	instance->set_aspect_ratio(width / float(height));
	glViewport(0, 0, width, height);
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

static void global_mouse_callback(GLFWwindow* window, double xpos, double ypos){
    renderer* instance = static_cast<renderer*>(glfwGetWindowUserPointer(window));
    instance->mouse_callback(xpos, ypos);
}

renderer::renderer(scene* target_scene, int width, int height, const char* name) {
    // TODO modify so that we can have two renderers?
    this->change_scene(target_scene);
    this->window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("Failed to initialize GLEW");
    }
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, global_framebuffer_size_callback);
    glfwSetKeyCallback(window, global_key_callback);
    glfwSetMouseButtonCallback(window, global_mouse_button_callback);
    glfwSetScrollCallback(window, global_scroll_callback);
    glfwSetCursorPosCallback(window, global_mouse_callback);
    
    this->aspect_ratio = width / float(height);
    this->focused = false;
    this->current_time = glfwGetTime();
    this->delta_time = 0.0;
    this->xpos = 0.0;
    this->ypos = 0.0;
    this->mv_forward = false;
    this->mv_backward = false;
    this->mv_left = false;
    this->mv_right = false;
    this->mv_up = false;
    this->mv_down = false;
}

renderer::~renderer() {
    glfwDestroyWindow(window);
}

void renderer::key_callback(int key, int scancode, int action, int mods){
    if(action == GLFW_REPEAT){
        return;
    }
    bool pressed = action == GLFW_PRESS;
    switch(key){
        case GLFW_KEY_ESCAPE:
            if(focused && action == GLFW_PRESS){
                focused = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;
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
    }
}

void renderer::mouse_button_callback(int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS){
            if(!focused){
                focused = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
}

void renderer::mouse_callback(double xpos, double ypos){
    ypos = -ypos;
    if(focused && (xpos != this->xpos || ypos != this->ypos)){
        target_camera.rotate_front(xpos - this->xpos, ypos - this->ypos);
        this->xpos = xpos;
        this->ypos = ypos;
    }
}

void renderer::scroll_callback(double xoffset, double yoffset){
    target_camera.zoom(-yoffset);
}

void renderer::run() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        if(focused){
            if(mv_forward){
                target_camera.move_forward(delta_time);
            } else if(mv_backward){
                target_camera.move_forward(-delta_time);
            }
            if(mv_left){
                target_camera.move_right(-delta_time);
            } else if(mv_right){
                target_camera.move_right(delta_time);
            }
            if(mv_up){
                target_camera.move_up(delta_time);
            } else if(mv_down){
                target_camera.move_up(-delta_time);
            }
        }
        {
            double new_time = glfwGetTime();
            delta_time = new_time - current_time;
            current_time = new_time;
        }
        
        target_scene->render(&target_camera, aspect_ratio);

        glfwSwapBuffers(window);
    }
}

void renderer::set_aspect_ratio(float aspect_ratio){
    this->aspect_ratio = aspect_ratio;
}

void renderer::change_scene(scene* new_scene){
    new_scene->init();
    target_scene = new_scene;
}
