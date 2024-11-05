
#include "texture.hpp"

#include <SOIL/SOIL.h>
#include <stdexcept>

texture::texture(const std::string& path, std::string name) : name(name){
    glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, &nr_channels, SOIL_LOAD_RGBA);
    if (image == nullptr) {
        throw std::runtime_error("Failed to load texture");
    }
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

texture::~texture(){
    glDeleteTextures(1, &texture_id);
}

void texture::set_active_texture(const shader* target_shader, int texture_unit) const{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    target_shader->apply_uniform(texture_unit, name);
}
