#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

/*!
 @brief Shader class to handle shader programs.
 @details This class is used to load and compile shader programs.
*/
class shader {
    private:
        GLuint program;
    public:
        /*!
         @brief Constructs a shader based on two paths
         @param vertex_path Path to the vertex shader
         @param fragment_path Path to the fragment shader
        */
        shader(const std::string vertex_path, const std::string fragment_path);
        ~shader();
        /*!
         @brief Use the shader program
         @warning If you don't forget to stop using the shader program, it will be used for all the following draw calls.
        */
        void use() const;
        /*!
         @brief Apply a uniform transformation matrix to the shader program with a given name
         @param matrix The matrix to apply
         @param name The name of the uniform variable in the shader program
        */
        void apply_uniform_mat4(glm::mat4 matrix, const std::string& name) const;
        /*!
         @brief Get the location of an attribute in the shader program
         @param name The name of the attribute
         @return The location of the attribute
        */
        GLint get_attrib_location(const std::string& name) const;
};
