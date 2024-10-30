#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <GL/glew.h>

static std::string read_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static GLuint compile_shader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        throw std::runtime_error(message);
    }

    return shader;
}

shader::shader(const std::string vertex_path, const std::string fragment_Path) {
    std::string vertex_source = read_file(vertex_path);
    std::string fragment_source = read_file(fragment_Path);

    GLuint vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        throw std::runtime_error(message);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

shader::~shader() {
    glDeleteProgram(program);
}

void shader::use() {
    glUseProgram(program);
}

void shader::apply_uniform_mat4(glm::mat4 matrix, const std::string& name) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, (float*)&matrix);
}
