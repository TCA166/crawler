#include "earth.hpp"

earth::earth(shader* target_shader, double xpos, double ypos, double zpos) : object(target_shader, "models/earth.obj", xpos, ypos, zpos), ground(texture("textures/ground.jpg")), ground_normal(texture("textures/ground_normal.png")), night(texture("textures/night.jpg")), clouds(texture("textures/clouds.png")){
    this->add_texture(&ground, "ground");
    this->add_texture(&ground_normal, "ground_normal");
    this->add_texture(&night, "night");
    this->add_texture(&clouds, "clouds");
}

earth::~earth(){
    
}
