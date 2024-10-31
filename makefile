
CFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp -Wall -Wextra -std=c++11
CC = g++

all: main

scene.o: src/scene.cpp src/scene.hpp
	$(CC) $(CFLAGS) -c src/scene.cpp

renderer.o: src/renderer.cpp src/renderer.hpp
	$(CC) $(CFLAGS) -c src/renderer.cpp

object.o: src/object.cpp src/object.hpp
	$(CC) $(CFLAGS) -c src/object.cpp

shader.o: src/shader.cpp src/shader.hpp
	$(CC) $(CFLAGS) -c src/shader.cpp

camera.o: src/camera.cpp src/camera.hpp
	$(CC) $(CFLAGS) -c src/camera.cpp

main: src/main.cpp shader.o camera.o object.o scene.o renderer.o
	$(CC) $(CFLAGS) -o main src/main.cpp shader.o camera.o object.o scene.o renderer.o

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel
