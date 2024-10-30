#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.hpp"
#include "camera.hpp"

#define WINDOW_NAME "Cralwer"
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

float aspectRatio = WINDOW_WIDTH / float(WINDOW_HEIGHT);
double xpos, ypos;
bool focused = false;
camera current_camera = camera();

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        if(focused){
            focused = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        focused = true;
    }
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

    GLuint VAO, VBO;
    shader box_shader = shader("src/shaders/box_vertex.glsl", "src/shaders/box_fragment.glsl");
    { // init box
        #include "Box.cpp"
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
        
        GLint posAttrib = 0;
	    GLint colAttrib = 1;
	
	    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(posAttrib);
	    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	    glEnableVertexAttribArray(colAttrib);
        
        glEnableVertexAttribArray(0);
    }
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(focused){
            double new_xpos, new_ypos;
            glfwGetCursorPos(window, &new_xpos, &new_ypos);
            current_camera.move_front(new_xpos - xpos, ypos - new_ypos);
            xpos = new_xpos;
            ypos = new_ypos;
            std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
        }
        { // render box
            box_shader.use();
            glm::mat4 view = current_camera.get_view_matrix();
            glm::mat4 projection = current_camera.get_projection_matrix(aspectRatio);
            
            glBindVertexArray(VAO);
            glm::mat4 transformation = projection * view;
            box_shader.apply_uniform_mat4(transformation, "transformation");
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);

            glUseProgram(0);

        }
        glfwSwapBuffers(window);
		glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}


