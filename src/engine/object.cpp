
#include "object.hpp"

#include "include.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <vector>
#include <iostream>

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
    this->scale = 1.0;
    this->xrot = 0.0;
    this->yrot = 0.0;
    this->zrot = 0.0;
    Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		throw std::runtime_error("ERROR::ASSIMP::" + std::string(import.GetErrorString()));
	}
    
    if(scene->mNumMeshes == 0){
        throw std::runtime_error("No meshes found in file");
    }

    const aiMesh* mesh = scene->mMeshes[0];

    for (unsigned int i = 0; i < mesh->mNumVertices; i++){
        data.push_back(mesh->mVertices[i].x);
        data.push_back(mesh->mVertices[i].y);
        data.push_back(mesh->mVertices[i].z);
        if (mesh->mNormals != nullptr) {
            data.push_back(mesh->mNormals[i].x);
            data.push_back(mesh->mNormals[i].y);
            data.push_back(mesh->mNormals[i].z);
        }
        else {
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
        if (mesh->mTextureCoords[0] != nullptr) {
            data.push_back(mesh->mTextureCoords[0][i].x);
            data.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
        if (mesh->mTangents != nullptr) {
            data.push_back(mesh->mTangents[i].x);
            data.push_back(mesh->mTangents[i].y);
            data.push_back(mesh->mTangents[i].z);
        }
        else {
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
        if (mesh->mBitangents != nullptr) {
            data.push_back(mesh->mBitangents[i].x);
            data.push_back(mesh->mBitangents[i].y);
            data.push_back(mesh->mBitangents[i].z);
        }
        else {
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
        }
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    vertex_count = mesh->mNumVertices;
}

object::~object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void object::init() {
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

    unsigned int vertexDataBufferSize = sizeof(float) * vertex_count * 3;
    unsigned int vertexTexBufferSize = sizeof(float) * vertex_count * 2;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexDataBufferSize * 4 + vertexTexBufferSize, data.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    index_count = indices.size();

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(normAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(tanAttrib, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(tanAttrib);
    glVertexAttribPointer(biTanAttrib, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(biTanAttrib);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
}

void object::render(const glm::mat4* viewProjection, glm::vec3 viewPos, const std::vector<const light*>& lights, glm::vec3 ambient) const {
    object_shader->use();

    size_t i = 0;
    for (const auto& pair : textures) {
        pair.second->set_active_texture(object_shader, i, pair.first);
        i++;
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, zpos)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale)) * glm::rotate(glm::mat4(1.0f), (float)xrot, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), (float)yrot, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), (float)zrot, glm::vec3(0.0f, 0.0f, 1.0f));

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
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture
    glUseProgram(0);
}

void object::render(const camera* target_camera, float aspect_ratio, const std::vector<const light*>& lights, glm::vec3 ambient) const{
    glm::mat4 viewProjection = target_camera->get_projection_matrix(aspect_ratio) * target_camera->get_view_matrix();
    this->render(&viewProjection, target_camera->get_position(), lights, ambient);
}

void object::add_texture(texture* tex, std::string name){
    textures[name] = tex;
}
