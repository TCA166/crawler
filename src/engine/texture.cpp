
#include "texture.hpp"

#include <SOIL/SOIL.h>

#include <stdexcept>

static void flip_y(unsigned char* img, int width, int height, int channels){
	int i, j;
	for( j = 0; j*2 < height; ++j ){
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;
		for( i = width * channels; i > 0; --i ){
			unsigned char temp = img[index1];
			img[index1] = img[index2];
			img[index2] = temp;
			++index1;
			++index2;
		}
	}
}

// by default flip the image, this is because SOIL loads the image upside down
texture::texture(const std::string& path) : texture(path, true) {}

texture::texture(const std::string& path, bool flip){
    glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, &nr_channels, SOIL_LOAD_AUTO);
    if (image == nullptr) {
        throw std::runtime_error(SOIL_last_result());
    }
	if(flip){
		flip_y(image, width, height, nr_channels);
	}
	int format = GL_RGB;
	if(nr_channels == 4){
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
}

texture::~texture(){
    glDeleteTextures(1, &texture_id);
}

void texture::set_active_texture(const shader* target_shader, int texture_unit, std::string name) const{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    target_shader->apply_uniform(texture_unit, name);
}
