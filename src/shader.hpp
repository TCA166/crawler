#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

class shader {
    private:
        GLuint program;
    public:
        shader(const std::string vertex_path, const std::string fragment_path);
        ~shader();
        void use();
        void apply_uniform_mat4(glm::mat4 matrix, const std::string& name);
};
