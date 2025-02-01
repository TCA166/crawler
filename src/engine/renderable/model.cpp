#include "model.hpp"

#ifndef STATIC_ASSETS
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#endif

#include <limits>
#include <stdexcept>
#include <stdio.h>

#define SHADER_VERTEX_POS 0
#define SHADER_TEX_POS 1

model::model(const std::vector<float> &data,
             const std::vector<unsigned int> &indices, glm::vec3 bounds,
             glm::vec3 negbounds)
    : data(data), indices(indices), vertex_count(data.size() / MODEL_LINE_SIZE),
      bounds(bounds), negbounds(negbounds) {}

#ifndef STATIC_ASSETS
model::model(const std::string &path) {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error("ERROR::ASSIMP::" +
                             std::string(import.GetErrorString()));
  }

  if (scene->mNumMeshes == 0) {
    throw std::runtime_error("No meshes found in file");
  }

  // Przetwórz wszystkie siatki
  for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
    const aiMesh *mesh = scene->mMeshes[m];

    if (mesh->mNumVertices == 0) {
      throw std::runtime_error("Mesh has no vertices!");
    }

float xbound = mesh->mVertices[0].x;
    float xnegbound = mesh->mVertices[0].x;
    float ybound = mesh->mVertices[0].y;
    float ynegbound = mesh->mVertices[0].y;
    float zbound = mesh->mVertices[0].z;
    float znegbound = mesh->mVertices[0].z;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      if (mesh->mVertices[i].x > xbound) {
        xbound = mesh->mVertices[i].x;
      }
      if (mesh->mVertices[i].x < xnegbound) {
        xnegbound = mesh->mVertices[i].x;
      }
      data.push_back(mesh->mVertices[i].x);

      if (mesh->mVertices[i].y > ybound) {
        ybound = mesh->mVertices[i].y;
      }
      if (mesh->mVertices[i].y < ynegbound) {
        ynegbound = mesh->mVertices[i].y;
      }
      data.push_back(mesh->mVertices[i].y);

      if (mesh->mVertices[i].z > zbound) {
        zbound = mesh->mVertices[i].z;
      }
      if (mesh->mVertices[i].z < znegbound) {
        znegbound = mesh->mVertices[i].z;
      }
      data.push_back(mesh->mVertices[i].z);

      if (mesh->mTextureCoords != nullptr &&
          mesh->mTextureCoords[0] != nullptr) {
        data.push_back(mesh->mTextureCoords[0][i].x);
        data.push_back(mesh->mTextureCoords[0][i].y);
      } else {
        data.push_back(0.0f);
        data.push_back(0.0f);
      }
    }

    this->bounds = glm::vec3(xbound, ybound, zbound);
    this->negbounds = glm::vec3(xnegbound, ynegbound, znegbound);

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    vertex_count += mesh->mNumVertices;
  }
}
#endif

model::~model() {}

glm::vec3 model::get_bounds() const { return bounds; }

glm::vec3 model::get_negbounds() const { return negbounds; }

void model::init() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               (sizeof(float) * vertex_count * 3) +
                   (sizeof(float) * vertex_count * 2),
               data.data(), GL_STATIC_DRAW);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(SHADER_VERTEX_POS, 3, GL_FLOAT, GL_FALSE,
                        MODEL_LINE_SIZE * sizeof(float), (void *)0);
  glEnableVertexAttribArray(SHADER_VERTEX_POS);
  glVertexAttribPointer(SHADER_TEX_POS, 2, GL_FLOAT, GL_FALSE,
                        MODEL_LINE_SIZE * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(SHADER_TEX_POS);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glEnableVertexAttribArray(0);
}

void model::deinit() const {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void model::draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
