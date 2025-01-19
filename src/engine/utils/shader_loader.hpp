
#pragma once

#include "../gl/shader.hpp"

#include <map>
#include <string>

/*!
 @brief A facility for loading shader code
 @details A singleton factory that loads shader code, thus improving load
 performance
*/
class shader_loader {
private:
  shader_loader();
  std::map<std::string, std::string> shaders;
  void add_shader(const std::string &key, unsigned char *data, size_t len);

public:
  /*!
   @brief Gets the singleton instance
   @return The singleton instance
  */
  static shader_loader &get();
  /*!
   @brief Loads a requested shader or retrieves it
   @param key the path to the shader
   @return The requested shader code
  */
  const std::string &load_shader(const std::string &key);
  ~shader_loader();
};
