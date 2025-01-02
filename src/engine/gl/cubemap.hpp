#pragma once

#include <vector>

#include "shader.hpp"

class cubemap {
  private:
    GLuint texture_id;

  public:
    cubemap(const std::vector<std::string> &paths);
    ~cubemap();
    void set_active_texture(const shader *target_shader, int texture_unit,
                            std::string name) const;
};
