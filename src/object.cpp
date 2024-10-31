
#include "object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <vector>

#define NO_EBO 0

#define SHADER_VERTEX_POSITION "vertexPosition"
#define SHADER_VERTEX_COLOR "vertexColor"
#define SHADER_VERTEX_TEXCOORD "vertexTexCoord"
#define SHADER_VERTEX_NORMAL "vertexNormal"

object::object(const shader* object_shader, const float* vertices_colors, size_t size) : object(object_shader, vertices_colors, size, 0.0, 0.0, 0.0) {}

object::object(const shader* object_shader, const float* vertices_colors, size_t size, double xpos, double ypos, double zpos) : xpos(xpos), ypos(ypos), zpos(zpos), object_shader(object_shader), size(size) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices_colors, GL_STATIC_DRAW);

    GLint posAttrib = object_shader->get_attrib_location(SHADER_VERTEX_POSITION);
    if(posAttrib == -1){
        throw std::runtime_error("Failed to find " SHADER_VERTEX_POSITION " attribute in shader");
    }
    GLint colAttrib = object_shader->get_attrib_location(SHADER_VERTEX_COLOR);
    if(colAttrib == -1){
        throw std::runtime_error("Failed to find " SHADER_VERTEX_COLOR " attribute in shader");
    }

    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(colAttrib);

    this->EBO = NO_EBO;

    glEnableVertexAttribArray(0);
}

object::object(const shader* object_shader, const std::string& path, double xpos, double ypos, double zpos) : xpos(xpos), ypos(ypos), zpos(zpos), object_shader(object_shader) {
    Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(import.GetErrorString()));
	}
    
    if(scene->mNumMeshes == 0){
        throw std::runtime_error("No meshes found in file");
    }

    // we allow for the shader to have either color or texture attributes
    GLint colAttrib = -1; // color attribute
    GLint normalAttrib = -1; // normal attribute
    GLint texAttrib = object_shader->get_attrib_location(SHADER_VERTEX_TEXCOORD); // texture attribute
    if(texAttrib == -1){ // if we don't have a texture attribute, we must have a color attribute
        colAttrib = object_shader->get_attrib_location(SHADER_VERTEX_COLOR);
        if(colAttrib == -1){ // if we don't have neither a texture nor a color attribute, we throw an error
            throw std::runtime_error("Failed to find " SHADER_VERTEX_TEXCOORD " and " SHADER_VERTEX_COLOR " attribute in shader");
        }
    } else {
        normalAttrib = object_shader->get_attrib_location(SHADER_VERTEX_NORMAL);
        if(normalAttrib == -1){
            throw std::runtime_error("Failed to find " SHADER_VERTEX_NORMAL " attribute in shader");
        }
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    aiMesh* mesh = scene->mMeshes[0];
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        vertices.push_back(1.0f); // Assuming w component is 1.0 for positions
        if(texAttrib != 1){ // if we have a texture attribute
            if(mesh->mTextureCoords[0] != nullptr){
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if(mesh->mNormals != nullptr){
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        } else { // if we have a color attribute
            if(mesh->mColors[0] != nullptr) {
                vertices.push_back(mesh->mColors[0][i].r);
                vertices.push_back(mesh->mColors[0][i].g);
                vertices.push_back(mesh->mColors[0][i].b);
                vertices.push_back(mesh->mColors[0][i].a);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }        
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLint posAttrib = object_shader->get_attrib_location(SHADER_VERTEX_POSITION);
    if(posAttrib == -1){
        throw std::runtime_error("Failed to find " SHADER_VERTEX_POSITION " attribute in shader");
    }    

    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    if(colAttrib != -1){
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(colAttrib);
    } else {
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(normalAttrib);
    }
    size = vertices.size() * sizeof(float);
}

object::~object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void object::render(const camera* target_camera, float aspect_ratio) const{
    object_shader->use();
    glm::mat4 view = target_camera->get_view_matrix();
    glm::mat4 projection = target_camera->get_projection_matrix(aspect_ratio);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));
    glm::mat4 transformation = projection * view * translation;
    object_shader->apply_uniform_mat4(transformation, "transformation");
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, size / (8 * sizeof(float)));
    if(EBO != NO_EBO){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}
