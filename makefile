
IFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -lSOIL
CFLAGS += -Wall -Wextra -Wpedantic -std=c++11 -g
CC = g++

all: main

texture.o: src/engine/texture.cpp src/engine/texture.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/texture.cpp

scene.o: src/engine/scene.cpp src/engine/scene.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/scene.cpp

renderer.o: src/engine/renderer.cpp src/engine/renderer.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/renderer.cpp

object.o: src/engine/object.cpp src/engine/object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/object.cpp

cube.o: src/engine/cube.cpp src/engine/cube.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/cube.cpp

shader.o: src/engine/shader.cpp src/engine/shader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/shader.cpp

light.o: src/engine/light.cpp src/engine/light.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/light.cpp

camera.o: src/engine/camera.cpp src/engine/camera.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/camera.cpp

engine.o: texture.o scene.o renderer.o object.o shader.o camera.o light.o cube.o
	$(CC) $(CFLAGS) -r texture.o scene.o renderer.o object.o shader.o camera.o light.o cube.o -o engine.o

entity.o: src/physics/entity.cpp src/physics/entity.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/physics/entity.cpp

physics.o: entity.o
	$(CC) $(CFLAGS) -r entity.o -o physics.o

game_object.o: src/game_object.cpp src/game_object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game_object.cpp

game.o: src/game.cpp src/game.hpp src/objects/* src/physics/constants.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game.cpp

main: src/main.cpp engine.o game.o physics.o game_object.o
	$(CC) $(IFLAGS) $(CFLAGS) -o main src/main.cpp engine.o game.o physics.o game_object.o

clean:
	rm -f *.o main

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel SOIL-devel glibc-devel.i686
