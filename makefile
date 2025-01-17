
IFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -lSOIL
CFLAGS += -Wall -Wextra -std=c++11 -g -pedantic -Wwrite-strings -Werror
CC = g++
EXE = main

all: main

engine.o: src/engine/makefile src/engine/*/*.cpp src/engine/*.hpp
	$(MAKE) -C src/engine IFLAGS="$(IFLAGS)" CFLAGS="$(CFLAGS)" CC="$(CC)"
	cp src/engine/engine.o .

entity.o: src/physics/entity.cpp src/physics/entity.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/physics/entity.cpp

physics.o: entity.o
	$(CC) $(CFLAGS) -r entity.o -o physics.o

game_object.o: src/game_object.cpp src/game_object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game_object.cpp

game.o: src/game.cpp src/game.hpp src/objects/* src/physics/constants.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game.cpp

main: src/main.cpp engine.o game.o physics.o game_object.o
	$(CC) $(CFLAGS) -o $(EXE) src/main.cpp engine.o game.o physics.o game_object.o $(IFLAGS)

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

webgl: CC=emcc
webgl: IFLAGS= -DWEBASM -I/usr/include/glm
webgl: CFLAGS= -std=c++11 -g
webgl: EXE=main.html
webgl: clean
webgl: main
webgl: clean
