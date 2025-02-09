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
#include "renderable/object.hpp"
#include "renderable/skybox.hpp"

#include "settings.hpp"

// Windows is very dumb so this define is here
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
