#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"

#define WINDOW_NAME "Crawler"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

float aspect_ratio = WINDOW_WIDTH / float(WINDOW_HEIGHT);
double xpos, ypos;
/*!
 @brief Whether the window is focused, used to determine if the camera should be moved
*/
bool focused = false;
/*!
 @brief The current camera used in the scene
*/
camera current_camera = camera();
double current_time = 0.0;
double delta_time = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	aspect_ratio = width / float(height);
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_W:
                current_camera.move_forward(delta_time);
                break;
            case GLFW_KEY_S:
                current_camera.move_forward(-delta_time);
                break;
            case GLFW_KEY_A:
                current_camera.move_right(-delta_time);
                break;
            case GLFW_KEY_D:
                current_camera.move_right(delta_time);
                break;
            case GLFW_KEY_SPACE:
                current_camera.move_up(delta_time);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                current_camera.move_up(-delta_time);
                break;
            case GLFW_KEY_ESCAPE:
                if(focused){
                    focused = false;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                break;

        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        focused = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    current_camera.zoom(-yoffset);
}

int main() {
    if(glfwInit() == GLFW_FALSE){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // tworzenie okna za pomocą glfw
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    shader box_shader = shader("src/shaders/box_vertex.glsl", "src/shaders/box_fragment.glsl");
    #include "Box.cpp"
    object box_obj = object(&box_shader, box, sizeof(box));
    object box_2 = object(&box_shader, box, sizeof(box), 1.0, 1.0);
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(focused){
            double new_xpos, new_ypos;
            glfwGetCursorPos(window, &new_xpos, &new_ypos);
            if(new_xpos != xpos || new_ypos != ypos){
                current_camera.rotate_front(new_xpos - xpos, ypos - new_ypos);
                xpos = new_xpos;
                ypos = new_ypos;
            }
        }
        { // handle time
            float new_time = glfwGetTime();
            delta_time = new_time - current_time;
            current_time = new_time;
        }
        box_obj.render(&current_camera, aspect_ratio);
        box_2.render(&current_camera, aspect_ratio);
        glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}


