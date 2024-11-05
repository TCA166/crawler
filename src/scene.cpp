
#include "scene.hpp"
#include <iostream>

scene::scene(glm::vec3 ambient_light) : ambient_light(ambient_light) {

}

scene::~scene() {
   
}

void scene::add_object(const object* obj) {
    objects.push_back(obj);
}

void scene::add_light(const light* light) {
    lights.push_back(light);
}

void scene::init() {
    
}

void scene::render(const camera* target_camera, float aspect_ratio) {
    glm::mat4 viewProjection = target_camera->get_projection_matrix(aspect_ratio) * target_camera->get_view_matrix();
    for(const object* obj : objects){
        obj->render(&viewProjection, lights);
    }
}

