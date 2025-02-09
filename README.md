# crawler

Computer Graphics class final project.

## Scene

A night scene in a forest, with multiple objects flocking in separate groups.
The "player" is free to roam this landscape with a shotgun. Any of the flying
objects once shot will disappear.

## Features

- [Shadow Mapping](./shaders/textured.frag)
- [Collision detection](./src/engine/utils/collision.cpp) (bounding box detection)
- [Procedurally generated terrain](./src/objects/random_floor.hpp)
- [Procedurally generated trees](./src/objects/tree.hpp)
- Multiple light source support
- Normal mapping
- [Flocking](./src/objects/boid.hpp) (altered algorithm featuring preferred y and multiple flocks)
- [Procedural texture generation](./src/objects/leaves.hpp)
- [Instancing](./src/engine/renderable/model.cpp)
- [Skybox](./src/engine/renderable/skybox.cpp)
- multithreading
- [separate window with a radar of boids](./src/scenes/radar.cpp)

## Compilation

### Linux

The best way to compile under Linux is to use Make. Just issue the following
command in project root:

```bash
make main
```

It should create a ```main``` executable file in project root. In case of
link errors try to install dependencies using the ```dependencies-dnf``` target
or ```dependencies-apt``` target.

### Windows

To compile this project on Windows, you need to ensure you have the necessary libraries, such as GLFW, GLEW, SOIL, GLM, GLUT and Assimp installed and linked correctly.

Ensure your project follows this directory structure:
```markdown 
/project-root
│── lib/          # Contains .lib files for linking
│── include/      # Contains header files (.h, .hpp) for dependencies
│── main.cpp      # Source file
```



## WASM

Here's how to compile and run this for Web Assembly:

1. Install [emscripten](https://emscripten.org/docs/getting_started/downloads.html)
2. Run ```source ./emsdk_env.sh``` in your install directory
3. Compile this using ```make main.html```
4. Run ```emrun main.html```

## Compile flags

Here are all available define flags that change program behavior:

1. ```WASM``` will disable OpenGL (GLFW, GLEW) calls that are unavailable in web
    environments
2. ```STATIC_ASSETS``` will make it so that ```SOIL``` and ```Assimp``` are no
    longer requirements, and all assets are loaded on compile time
3. ```NO_THREADS``` will disable threading, reducing performance, but improving
    compatibility
