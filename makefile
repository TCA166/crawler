
IFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -lSOIL
CFLAGS += -Wall -Wextra -std=c++11 -g -pedantic -Wwrite-strings -Werror -pg
CC = g++

all: main

game.o: src/scenes/game.cpp src/scenes/game.hpp src/objects/*.hpp src/physics/constants.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/scenes/game.cpp

radar.o: src/scenes/radar.cpp src/scenes/radar.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/scenes/radar.cpp

scenes.o: game.o radar.o
	$(CC) $(CFLAGS) -r game.o radar.o -o scenes.o

engine.o: src/engine/makefile src/engine/*/*.cpp src/engine/*.hpp
	$(MAKE) -C src/engine IFLAGS="$(IFLAGS)" CFLAGS="$(CFLAGS)" CC="$(CC)"
	cp src/engine/engine.o .

entity.o: src/physics/entity.cpp src/physics/entity.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/physics/entity.cpp

physics.o: entity.o
	$(CC) $(CFLAGS) -r entity.o -o physics.o

game_object.o: src/game_object.cpp src/game_object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game_object.cpp

main: src/main.cpp engine.o scenes.o physics.o game_object.o
	$(CC) $(CFLAGS) -o main src/main.cpp engine.o scenes.o physics.o game_object.o $(IFLAGS)

clean:
	rm -f *.o main
	$(MAKE) -C src/engine clean

doc: doc/Doxyfile src/*/*.cpp src/*.hpp src/*/*.hpp src/*/*.cpp
	mkdir -p doc/build
	doxygen doc/Doxyfile

doc-pdf: doc
	$(MAKE) -C doc/build/latex all
	cp doc/build/latex/refman.pdf refman.pdf

dependencies-dnf:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel SOIL-devel glibc-devel.i686

dependencies-apt:
	sudo apt install -y libglm-dev libglew-dev libglfw3 libglfw3-dev libsoil-dev libassimp-dev

static-shaders: shaders/*.*
	mkdir -p static/shaders
	for file in $^ ; do \
		xxd -i $$file static/shaders/$$(basename $$file).h ; \
	done

static-textures: textures/*.* textures/*/*.*
	mkdir -p static/textures
	for file in $^ ; do \
		xxd -i $$file static/textures/$$(basename $$file | cut -d. -f1).h ; \
	done

c-assets: static-shaders static-textures

main.html: CC=emcc
main.html: IFLAGS= -DSTATIC_ASSETS -DWASM -DNO_THREADS -I/usr/include/glm
main.html: CFLAGS= -std=c++11 -g -Og
main.html: c-assets src/main.cpp engine.o game.o physics.o game_object.o
# emcc really doesn't like the -r flag, so we have to compile everything in one go
	$(CC) $(CFLAGS) -sNO_DISABLE_EXCEPTION_CATCHING -sUSE_GLFW=3 -sASSERTIONS -sUSE_WEBGL2=1 -sFULL_ES3=1 --emrun --use-port=contrib.glfw3 -o main.html src/main.cpp src/engine/camera.o src/engine/collision.o src/engine/cube.o src/engine/cubemap.o src/engine/image_loader.o src/engine/light.o src/engine/model.o src/engine/model_loader.o src/engine/shader_loader.o src/engine/object.o src/engine/renderer.o src/engine/shader.o src/engine/skybox.o src/engine/texture.o src/engine/triangle.o src/engine/scene.o src/engine/wall.o game.o entity.o game_object.o $(IFLAGS)
