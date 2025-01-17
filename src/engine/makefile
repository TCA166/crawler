
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

gl.o: renderer.o texture.o shader.o cubemap.o
	$(CC) $(CFLAGS) -r renderer.o texture.o shader.o cubemap.o -o gl.o

# renderable subfolder

cube.o: renderable/cube.cpp renderable/cube.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/cube.cpp

object.o: renderable/object.cpp renderable/object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/object.cpp

model.o: renderable/model.cpp renderable/model.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/model.cpp

skybox.o: renderable/skybox.cpp renderable/skybox.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/skybox.cpp

wall.o: renderable/wall.cpp renderable/wall.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/wall.cpp

triangle.o: renderable/triangle.cpp renderable/triangle.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/triangle.cpp

renderable.o: cube.o object.o skybox.o wall.o triangle.o model.o
	$(CC) $(CFLAGS) -r cube.o object.o skybox.o wall.o model.o triangle.o -o renderable.o

# scene subfolder

scene.o: scene/scene.cpp scene/scene.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/scene.cpp

light.o: scene/light.cpp scene/light.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/light.cpp

camera.o: scene/camera.cpp scene/camera.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c scene/camera.cpp

scene_m.o: scene.o light.o camera.o
	$(CC) $(CFLAGS) -r scene.o light.o camera.o -o scene_m.o

# utils subfolder

collision.o: utils/collision.cpp utils/collision.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/collision.cpp

model_loader.o: utils/model_loader.cpp utils/model_loader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/model_loader.cpp

image_loader.o: utils/image_loader.cpp utils/image_loader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/image_loader.cpp

utils.o: collision.o model_loader.o image_loader.o
	$(CC) $(CFLAGS) -r collision.o model_loader.o image_loader.o -o utils.o

# complete engine

engine.o: gl.o renderable.o scene_m.o utils.o
	$(CC) $(CFLAGS) -r gl.o renderable.o scene_m.o utils.o -o engine.o

clean:
	rm -f *.o
