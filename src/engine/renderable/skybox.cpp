#include "skybox.hpp"

skybox::skybox(const shader *skybox_shader, std::vector<std::string> &paths)
    : cube(skybox_shader, 0.0, 0.0, 0.0) {
    skybox_texture = new cubemap(paths);
    this->add_texture(skybox_texture, "skybox");
    this->set_scale(100.0);
}

skybox::~skybox() { delete skybox_texture; }

void skybox::render(const glm::mat4 *view_projection, glm::vec3 camera_position,
                    const std::vector<const light *> &lights,
                    glm::vec3 ambient_light) const {
    glDepthFunc(GL_LEQUAL);
    object::render(view_projection, camera_position, lights, ambient_light);
    glDepthFunc(GL_LESS);
}

void skybox::init() { object::init(); }
