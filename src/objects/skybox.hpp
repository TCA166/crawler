#pragma once

#include "../engine/engine.hpp"

class skybox : public cube {
  private:
    cubemap *skybox_texture;

  public:
    skybox();
    ~skybox();
};

inline skybox::skybox()
    : cube(new shader("shaders/skybox_vertex.glsl",
                      "shaders/skybox_fragment.glsl"),
           0.0, 0.0, 0.0) {
    std::vector<std::string> paths = {
        TEXTURE_PATH("skybox/right.png"), TEXTURE_PATH("skybox/left.png"),
        TEXTURE_PATH("skybox/top.png"),   TEXTURE_PATH("skybox/bottom.png"),
        TEXTURE_PATH("skybox/front.png"), TEXTURE_PATH("skybox/back.png")};
    skybox_texture = new cubemap(paths);
    this->add_texture(skybox_texture, "skybox");
    this->set_scale(100.0);
}

inline skybox::~skybox() { delete skybox_texture; }
