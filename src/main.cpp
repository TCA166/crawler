#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"

#define WINDOW_NAME "Cralwer"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#define VERTEX_SHADER_PATH "src/shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH "src/shaders/fragment.glsl"

int main() {
    glfwInit();
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
    glewInit();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    shader current_shader = shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    while(!glfwWindowShouldClose(window)){
		
		glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}


