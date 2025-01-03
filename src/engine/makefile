
all: engine.o

# gl subfolder

renderer.o: gl/renderer.cpp gl/renderer.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c gl/renderer.cpp

texture.o: gl/texture.cpp gl/texture.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c gl/texture.cpp

shader.o: gl/shader.cpp gl/shader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c gl/shader.cpp

cubemap.o: gl/cubemap.cpp gl/cubemap.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c gl/cubemap.cpp

# renderable subfolder

cube.o: renderable/cube.cpp renderable/cube.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/cube.cpp

object.o: renderable/object.cpp renderable/object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/object.cpp

skybox.o: renderable/skybox.cpp renderable/skybox.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/skybox.cpp

# scene subfolder

scene.o: scene/scene.cpp scene/scene.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/scene.cpp

light.o: scene/light.cpp scene/light.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/light.cpp

camera.o: scene/camera.cpp scene/camera.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/camera.cpp

engine.o: texture.o scene.o renderer.o object.o shader.o camera.o light.o cube.o cubemap.o skybox.o
	$(CC) $(CFLAGS) -r texture.o scene.o renderer.o object.o shader.o camera.o light.o cube.o cubemap.o skybox.o -o engine.o

clean:
	rm -f *.o
