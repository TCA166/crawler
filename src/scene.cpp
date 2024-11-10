#include "scene.hpp"

#include <iostream>

scene::scene(glm::vec3 ambient_light, glm::vec3 background_color) : ambient_light(ambient_light), background_color(background_color) {

}

scene::~scene() {
   
}

void scene::add_object(object* obj) {
    objects.push_back(obj);
}

void scene::add_light(light* light) {
    lights.push_back(light);
}

void scene::init() {
    for(object* obj : objects){
        obj->init();
    }
}

void scene::render(const camera* target_camera, float aspect_ratio) {
    glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 viewProjection = target_camera->get_projection_matrix(aspect_ratio) * target_camera->get_view_matrix();
    for(const object* obj : objects){
        obj->render(&viewProjection, target_camera->get_position(), (const std::vector<const light*>&)lights, ambient_light);
    }
}

