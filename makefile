
IFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -lSOIL
CFLAGS += -Wall -Wextra -Wpedantic -std=c++11 -g
CC = g++

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
	$(CC) $(CFLAGS) -o main src/main.cpp engine.o game.o physics.o game_object.o $(IFLAGS)

clean:
	rm -f *.o main
	$(MAKE) -C src/engine clean

doc: doc/Doxyfile
	mkdir -p doc/build
	doxygen doc/Doxyfile

doc-pdf: doc
	$(MAKE) -C doc/build/latex all
	cp doc/build/latex/refman.pdf refman.pdf

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel SOIL-devel glibc-devel.i686
