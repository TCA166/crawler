
#pragma once

#include "object.hpp"

/*!
 @brief A gun object
*/
class gun : public object {
public:

  gun(const shader *object_shader, const std::string &path, double xpos,
      double ypos, double zpos);
  ~gun();
};
