#pragma once

// All the underlying lib headers
#include "include.hpp"
// gl folder
#include "gl/cubemap.hpp"
#include "gl/renderer.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
// scene folder
#include "scene/camera.hpp"
#include "scene/light.hpp"
#include "scene/scene.hpp"
// renderable folder
#include "renderable/cube.hpp"
#include "renderable/object.hpp"
#include "renderable/skybox.hpp"
#include "renderable/wall.hpp"

#define TEXTURE_PATH(name) "textures/" name
#define SHADER_PATH(name) "shaders/" name
#define MODEL_PATH(name) "models/" name
