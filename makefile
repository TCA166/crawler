
CFLAGS += -lGL -lGLU -lGLEW -lglfw -lassimp
CC = g++

all: main

shader.o: src/shader.cpp src/shader.hpp
	$(CC) $(CFLAGS) -c src/shader.cpp

main: src/main.cpp shader.o
	$(CC) $(CFLAGS) -o main src/main.cpp shader.o

dependencies:
	sudo dnf install -y glfw-devel assimp-devel glew-devel glm-devel
