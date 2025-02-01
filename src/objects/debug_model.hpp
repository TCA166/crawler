#pragma once

#include "../engine/engine.hpp"
#include "../engine/renderable/model.hpp"
#include "../engine/utils/model_loader.hpp"

/**
 @brief A generic debug model with a texture and normal map
*/
class debug_model : public object {
public:
  /**
   @brief Constructs a debug model object
   @param object_shader The shader to use for rendering
   @param tex The texture of the object
   @param norm The normal map of the object
   @param xpos The x position of the object
   @param ypos The y position of the object
   @param zpos The z position of the object
  */
  debug_model(const shader *object_shader, const std::string &model_path,
              const texture *tex,
              const texture *norm, double xpos, double ypos, double zpos);
  ~debug_model();
};

inline debug_model::debug_model(const shader *object_shader,
                                const std::string &model_path,
                                const texture *tex,
                                const texture *norm, double xpos, double ypos,
                                double zpos)

    : object(object_shader, model_loader::get().get_model(model_path), xpos, ypos,
             zpos) {
  this->add_texture(tex, "texture0");
  this->add_texture(norm, "normal0");
}

inline debug_model::~debug_model() { /* Custom cleanup can be added here */
}