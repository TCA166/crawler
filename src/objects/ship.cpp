
#include "ship.hpp"

ship::ship(shader* textured_shader, double xpos, double ypos, double zpos) : object(textured_shader, "models/spaceship.obj", xpos, ypos, zpos), body(texture("textures/spaceship.jpg")), normal(texture("textures/spaceship_normal.jpg")){
    this->add_texture(&body, "texture0");
    this->add_texture(&normal, "normal0");
}

ship::~ship(){
    
}
