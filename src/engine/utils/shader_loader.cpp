#include "shader_loader.hpp"

#include "../settings.hpp"

#include <stdexcept>
#ifndef STATIC_ASSETS

#include <fstream>
#include <sstream>

/*!
 @brief Read a file and return its contents as a string
 @param file_path The path to the file
 @return The contents of the file as a string
*/
static std::string read_file(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + file_path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

#else
#include "../../../static/shaders/light_pass.frag.h" // shaders_light_pass_frag
#include "../../../static/shaders/light_pass.vert.h"
#include "../../../static/shaders/simple_textured.frag.h"
#include "../../../static/shaders/skybox.frag.h"
#include "../../../static/shaders/skybox.vert.h"
#include "../../../static/shaders/textured.frag.h"
#include "../../../static/shaders/textured.vert.h"
#include "../../../static/shaders/triangle.frag.h"
#include "../../../static/shaders/triangle.vert.h"
#endif

shader_loader::shader_loader() {
#ifdef STATIC_ASSETS

  add_shader(SHADER_PATH("triangle.vert"), shaders_triangle_vert,
             shaders_triangle_vert_len);
  add_shader(SHADER_PATH("triangle.frag"), shaders_triangle_frag,
             shaders_triangle_frag_len);

  add_shader(SHADER_PATH("textured.vert"), shaders_textured_vert,
             shaders_textured_vert_len);
  add_shader(SHADER_PATH("textured.frag"), shaders_textured_frag,
             shaders_textured_frag_len);

  add_shader(SHADER_PATH("simple_textured.frag"), shaders_simple_textured_frag,
             shaders_simple_textured_frag_len);

  add_shader(SHADER_PATH("skybox.vert"), shaders_skybox_vert,
             shaders_skybox_vert_len);
  add_shader(SHADER_PATH("skybox.frag"), shaders_skybox_frag,
             shaders_skybox_frag_len);

  add_shader(SHADER_PATH("light_pass.vert"), shaders_light_pass_vert,
             shaders_light_pass_vert_len);
  add_shader(SHADER_PATH("light_pass.frag"), shaders_light_pass_frag,
             shaders_light_pass_frag_len);

#endif
}

void shader_loader::add_shader(const std::string &key, unsigned char *data,
                               size_t len) {
  std::string shader(reinterpret_cast<char *>(data), len);
#ifdef WASM
  // replace the first line
  size_t pos = shader.find("\n");
  shader.replace(0, pos, "#version 300 es");
#endif
  shaders[key] = shader;
}

shader_loader &shader_loader::get() {
  static shader_loader instance;
  return instance;
}

const std::string &shader_loader::load_shader(const std::string &key) {
  try {
    return shaders.at(key);
  } catch (const std::out_of_range &e) {
#ifndef STATIC_ASSETS
    shaders[key] = read_file(key);
    return shaders[key];
#else
    throw std::runtime_error("Shader not found");
#endif
  }
}

shader_loader::~shader_loader() {}
