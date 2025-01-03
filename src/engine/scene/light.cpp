#include "light.hpp"

#include "../settings.hpp"

light::light(glm::vec3 position, glm::vec3 direction, glm::vec3 color)
    : position(position), direction(direction), color(color) {
    // create the framebuffer
    glGenFramebuffers(1, &depthMapFBO);
    // create the texture to store the depth values
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_RES, SHADOW_RES,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach the texture to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

light::~light() {}

const glm::vec3 &light::get_direction() const { return direction; }

const glm::vec3 &light::get_color() const { return color; }

const glm::vec3 &light::get_position() const { return position; }

const glm::mat4 light::get_light_view() const {
    return glm::lookAt(this->get_position(), this->get_direction(),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

void light::set_direction(glm::vec3 direction) { this->direction = direction; }

void light::set_color(glm::vec3 color) { this->color = color; }

void light::bind_depth_map() const {
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void light::use_depth_map(int texture_unit) const {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}
