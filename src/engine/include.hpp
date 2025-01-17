
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef WEBASM // this define toggles the use of SOIL and Assimp
#include <glm/ext.hpp>
#else
#include <ext.hpp>
#endif
