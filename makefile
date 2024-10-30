
CFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp
CC = g++

all: main

shader.o: src/shader.cpp src/shader.hpp
	$(CC) $(CFLAGS) -c src/shader.cpp

camera.o: src/camera.cpp src/camera.hpp
	$(CC) $(CFLAGS) -c src/camera.cpp

main: src/main.cpp shader.o camera.o
	$(CC) $(CFLAGS) -o main src/main.cpp shader.o camera.o

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel
