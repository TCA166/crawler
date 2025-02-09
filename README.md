# crawler

Computer Graphics class project.

## Features

- [Shadow Mapping](./shaders/textured.frag)
- [Collision detection](./src/engine/utils/collision.cpp) (bounding box detection)
- [Procedurally generated terrain](./src/objects/random_floor.hpp)
- [Procedurally generated trees](./src/objects/tree.hpp)
- Multiple light source support
- Normal mapping
- [Flocking](./src/objects/boid.hpp)
- [Procedural texture generation](./src/objects/leaves.hpp)
- [Instancing](./src/engine/renderable/model.cpp)
- [Skybox](./src/engine/renderable/skybox.cpp)

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
