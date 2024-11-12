
IFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -lSOIL
CFLAGS += -Wall -Wextra -std=c++11 -g
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

shader.o: src/engine/shader.cpp src/engine/shader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/shader.cpp

camera.o: src/engine/camera.cpp src/engine/camera.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/engine/camera.cpp

engine.o: texture.o scene.o renderer.o object.o shader.o camera.o
	$(CC) $(CFLAGS) -r texture.o scene.o renderer.o object.o shader.o camera.o -o engine.o

game.o: src/game.cpp src/game.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/game.cpp

ship.o: src/objects/ship.cpp src/objects/ship.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/objects/ship.cpp

earth.o: src/objects/earth.cpp src/objects/earth.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c src/objects/earth.cpp

objects.o: ship.o earth.o
	$(CC) $(CFLAGS) -r ship.o earth.o -o objects.o

main: src/main.cpp engine.o game.o objects.o
	$(CC) $(IFLAGS) $(CFLAGS) -o main src/main.cpp engine.o game.o objects.o

clean:
	rm -f *.o main

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel SOIL-devel glibc-devel.i686
