# crawler

Computer Graphics class final project.

## Scene

A night scene in a forest, with multiple objects flocking in separate groups.
The "player" is free to roam this landscape with a shotgun. Any of the flying
objects once shot will disappear.

## Features

- [Shadow Mapping](./shaders/textured.frag)
- [Collision detection](./src/engine/utils/collision.cpp) (bounding box
    detection)
- [Procedurally generated terrain](./src/objects/random_floor.hpp)
- [Procedurally generated trees](./src/objects/tree.hpp)
- Multiple light source support
- Normal mapping
- [Flocking](./src/objects/boid.hpp) (altered algorithm featuring preferred y
    and multiple flocks)
- [Procedural texture generation](./src/objects/leaves.hpp)
- [Instancing](./src/engine/renderable/model.cpp)
- [Skybox](./src/engine/renderable/skybox.cpp)
- multithreading
- [separate window with a radar of boids](./src/scenes/radar.cpp)

### Engine

Our project features a fully fledged out engine, very much capable and ready
for possible future re-use. We have implemented a whole class dependency
structure, which did indeed help during development. The engine is
multi-threaded, does support multiple windows, and even viewing the same scene
from two different cameras from two different windows. The full documentation
for this engine can be found [here](https://tca166.github.io/crawler/).

#### Pipeline

In our engine a pretty standard rendering pipeline was implemented.
For each window a separate `renderer` instance exists. This renderer has
an attached scene. Renderer handles all the I/O, and interacts with the window,
creating a nice abstract environment for the `scene` to render it's contents.
The scene handles in broad strokes how everything is rendered. It mandates a
shadow pass, which objects are rendered etc. During rendering the objects
are rendered in bulk, in accordance with their shader, minimizing shader state
switches and uniform passing operations. A `scene` provides the objects, chosen
to be rendered with a nice abstraction, providing them with an already loaded
shader with passed view and projection matrices and other necessary information.
An `object` when being rendered usually just passes it's model matrices to the
shader and then calls draw on the `model` it holds. The `model` class handles
all the nitty-gritty OpenGL buffer handling, and also has a subclass
`model_instanced` that supports instanced rendering. With this pipeline we have
a very modular system that allows for very easy modification.

#### Asset Loading

Within our engine we have adopted the use of centralized model loading
facilities. With these factory singletons we can easily cache required
images, models and shader, while also allowing for static asset storage,
something that our engine does indeed allow for.

#### Collision detection

Within our engine we have adopted bounding box collision detection. However
we don't require our bounds to be axis aligned. Depending on the situation,
however we can easily combine the colliders, thanks to the modularity of the
engine. It's as simple as overriding the method related to collision and then,
for example, checking two different box colliders depending on position.

### Flocking

We have implemented flocking within our project. The boids move accordingly
with forces that act upon them. Then we simply adjust the forces based on
the position of nearby boids and their velocity. Apart from that we have also
added species into the algorithm, meaning that boids will stick to their own
species, avoiding foreign boids more than they like to keep to their own,
and also added specific preferred y levels for boid species and random
perturbations, meaning that the movements shouldn't become too static.

### Procedural generation

Our project utilizes a scene that is almost entirely procedurally generated.
The terrain is generated using a 2D perlin noise map, and the trees are
also procedurally generated, with the instanced leaves utilizing the same
noise for a texture.

## Compilation

The source code, *should*, be platform independent. It was tested on both
Windows and Linux and C++11 compliant.

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

To compile this project on Windows, you need to ensure you have the necessary
libraries, such as GLFW, GLEW, SOIL, GLM, GLUT and Assimp installed and linked
correctly. Ensure your project follows this directory structure:

```markdown
/project-root
|- lib/          # Contains .lib files for linking
|- include/      # Contains header files (.h, .hpp) for dependencies
|- main.cpp      # Source file
```

Realistically speaking compiling this on Windows is such a hassle. We did
experiment with running this project in WSL, and caused a segfault in the
virtual graphics driver.

## WASM

We did experiment with compiling and running this project under Web Assembly.
Our project does compile for web assembly, and in theory we are entirely
standard compliant and should be able to run under WASM. However due to minor
technical differences (and not so minor ones) the runtime always seemed to stall
on load. Lack of threading and cost associated with virtualization certainly
didn't help. We speculate, that in order to properly run a project of this scale
under web assembly, a low level redesign of the pipeline would be needed. In
short: one would need to build an engine for WASM first, everything else second.
However, the technical backend for running this in WASM is here.
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

## License

This project is licensed under GPLv3, this includes the code (bar for a single
explicit exception) in C++ and GLSL. The wavefront and image assets belong to
their respective copyright owners.
