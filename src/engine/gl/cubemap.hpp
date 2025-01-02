#pragma once

#include <vector>

#include "shader.hpp"

class cubemap : public texture {
  public:
    cubemap(const std::vector<std::string> &paths);
    ~cubemap();
};
