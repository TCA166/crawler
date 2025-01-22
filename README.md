# crawler

Computer Graphics class project.

Skybox from: [Emil Persson](https://opengameart.org/content/field-skyboxes)

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
