
#include "object.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <stdexcept>

object::object(const shader* object_shader, const float* vertices_colors, size_t size) : object(object_shader, vertices_colors, size, 0.0, 0.0) {}

object::object(const shader* object_shader, const float* vertices_colors, size_t size, double xpos, double ypos) {
    this->object_shader = object_shader;
    this->xpos = xpos;
    this->ypos = ypos;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices_colors, GL_STATIC_DRAW);

    GLint posAttrib = object_shader->get_attrib_location("vertexPosition");
    if(posAttrib == -1){
        throw std::runtime_error("Failed to find position attribute in shader");
    }
    GLint colAttrib = object_shader->get_attrib_location("vertexColor");
    if(colAttrib == -1){
        throw std::runtime_error("Failed to find color attribute in shader");
    }

    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    glEnableVertexAttribArray(0);
}

object::~object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void object::render(const camera* target_camera, float aspect_ratio) {
    object_shader->use();
    glm::mat4 view = target_camera->get_view_matrix();
    glm::mat4 projection = target_camera->get_projection_matrix(aspect_ratio);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0.0f));

    glBindVertexArray(VAO);
    glm::mat4 transformation = projection * view * translation;
    object_shader->apply_uniform_mat4(transformation, "transformation");
    glDrawArrays(GL_TRIANGLES, 0, 36); // TODO is this always 36?
    glBindVertexArray(0);
}
