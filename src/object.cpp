
#include "object.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <vector>
#include <iostream>

#define NO_EBO 0

#define SHADER_VERTEX_POSITION "vertexPosition"
#define SHADER_VERTEX_COLOR "vertexColor"
#define SHADER_VERTEX_TEXCOORD "vertexTexCoord"
#define SHADER_VERTEX_NORMAL "vertexNormal"
#define SHADER_VERTEX_TANGENT "vertexTangent"
#define SHADER_VERTEX_BITANGENT "vertexBitangent"

object::object(const shader* object_shader, const std::string& path, double xpos, double ypos, double zpos) {
    this->object_shader = object_shader;
    this->xpos = xpos;
    this->ypos = ypos;
    this->zpos = zpos;
    Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(import.GetErrorString()));
	}
    
    if(scene->mNumMeshes == 0){
        throw std::runtime_error("No meshes found in file");
    }

    // we allow for the shader to have either color or texture attributes
    GLint posAttrib = object_shader->get_attrib_location(SHADER_VERTEX_POSITION);
    if(posAttrib == -1){
        throw std::runtime_error("Failed to find " SHADER_VERTEX_POSITION " attribute in shader");
    }
    GLint texAttrib = object_shader->get_attrib_location(SHADER_VERTEX_TEXCOORD);
    if(texAttrib == -1){
        texAttrib = 1;
    }
    GLint normAttrib = object_shader->get_attrib_location(SHADER_VERTEX_NORMAL);
    if(normAttrib == -1){
        normAttrib = 2;
    }
    GLint tanAttrib = object_shader->get_attrib_location(SHADER_VERTEX_TANGENT);
    if(tanAttrib == -1){
        tanAttrib = 3;
    }
    GLint biTanAttrib = object_shader->get_attrib_location(SHADER_VERTEX_BITANGENT);
    if(biTanAttrib == -1){
        biTanAttrib = 4;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    std::vector<float> textureCoord;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        if (mesh->mTextureCoords[0] != nullptr) {
            textureCoord.push_back(mesh->mTextureCoords[0][i].x);
            textureCoord.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            textureCoord.push_back(0.0f);
            textureCoord.push_back(0.0f);
        }
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    unsigned int vertexDataBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    vertex_count = mesh->mNumVertices;
    unsigned int vertexNormalBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexTexBufferSize = sizeof(float) * mesh->mNumVertices * 2;
    unsigned int vertexTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexBiTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataBufferSize, mesh->mVertices);
    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize, vertexNormalBufferSize, mesh->mNormals);
    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize, vertexTexBufferSize, textureCoord.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize, vertexTangentBufferSize, mesh->mTangents);
    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize, vertexBiTangentBufferSize, mesh->mBitangents);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    index_count = indices.size();

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)(uintptr_t)(vertexDataBufferSize));
    glEnableVertexAttribArray(normAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)(uintptr_t)(vertexDataBufferSize + vertexNormalBufferSize));
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(tanAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)(uintptr_t)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize));
    glEnableVertexAttribArray(tanAttrib);
    glVertexAttribPointer(biTanAttrib, 3, GL_FLOAT, GL_FALSE, 0, (void*)(uintptr_t)(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize));
    glEnableVertexAttribArray(biTanAttrib);

    glEnableVertexAttribArray(0);
}

object::~object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if(EBO != NO_EBO){
        glDeleteBuffers(1, &EBO);
    }
}

void object::render(const glm::mat4* viewProjection, glm::vec3 viewPos, const std::vector<const light*>& lights, glm::vec3 ambient) const {
    object_shader->use();

    for(size_t i = 0; i < textures.size(); i++){
        textures[i]->set_active_texture(object_shader, i);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos));

    object_shader->apply_uniform_mat4(model, "model");
    object_shader->apply_uniform_mat4(*viewProjection, "viewProjection");
    object_shader->apply_uniform_vec3(ambient, "ambientLight");
    object_shader->apply_uniform_vec3(viewPos, "viewPos");
    object_shader->apply_uniform_scalar(0.1f, "shininess");

    // Pass light properties to the shader
    for (size_t i = 0; i < lights.size(); ++i) {
        std::string name = "lights[" + std::to_string(i) + "]";
        object_shader->apply_light(lights[i], name);
    }
    object_shader->apply_uniform(lights.size(), "numLights");

    glBindVertexArray(VAO);
    if (EBO != NO_EBO) {
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

void object::render(const camera* target_camera, float aspect_ratio, const std::vector<const light*>& lights, glm::vec3 ambient) const{
    glm::mat4 viewProjection = target_camera->get_projection_matrix(aspect_ratio) * target_camera->get_view_matrix();
    this->render(&viewProjection, target_camera->get_position(), lights, ambient);
}

void object::add_texture(texture* tex){
    textures.push_back(tex);
}
