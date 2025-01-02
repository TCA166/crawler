
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

game.o: src/game.cpp src/game.hpp src/objects/*.cpp src/physics/constants.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game.cpp

main: src/main.cpp engine.o game.o physics.o game_object.o
	$(CC) $(IFLAGS) $(CFLAGS) -o main src/main.cpp engine.o game.o physics.o game_object.o

clean:
	rm -f *.o main
	$(MAKE) -C src/engine clean

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel SOIL-devel glibc-devel.i686
