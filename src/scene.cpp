
#include "scene.hpp"
#include <iostream>

scene::scene() {
}

scene::~scene() {
   
}

void scene::add_object(const object& obj) {
    objects.push_back(&obj);
}

void scene::init() {
    
}

void scene::render(const camera* target_camera, float aspect_ratio) {
    for(const object* obj : objects){
        obj->render(target_camera, aspect_ratio);
    }
}

