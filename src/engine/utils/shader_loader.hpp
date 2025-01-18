
#pragma once

#include "../gl/shader.hpp"

#include <map>
#include <string>

class shader_loader {
private:
  shader_loader();
  std::map<std::string, std::string> shaders;
  void add_shader(const std::string &key, unsigned char *data, size_t len);

public:
  static shader_loader &get();
  const std::string &load_shader(const std::string &key);
  ~shader_loader();
};
