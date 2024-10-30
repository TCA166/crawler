#pragma once

#include <GL/glew.h>
#include <string>

class shader {
    private:
        GLuint program;
    public:
        shader(const std::string vertex_path, const std::string fragment_path);
        ~shader();
};
