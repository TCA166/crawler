#pragma once

#define TEXTURE_PATH(name) "textures/" name
#define SHADER_PATH(name) "shaders/" name
#define MODEL_PATH(name) "models/" name

#define SHADOW_RES 1024

enum axes { X, Y, Z };

#define RENDER_MIN 0.01f
#define RENDER_MAX 100000.0f

#define UP glm::vec3(0.0f, 1.0f, 0.0f)
