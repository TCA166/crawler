
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

object.o: renderable/object.cpp renderable/object.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/object.cpp

model.o: renderable/model.cpp renderable/model.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/model.cpp

skybox.o: renderable/skybox.cpp renderable/skybox.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c renderable/skybox.cpp

renderable.o: object.o skybox.o model.o
	$(CC) $(CFLAGS) -r object.o skybox.o model.o -o renderable.o

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

noise.o: utils/noise.cpp utils/noise.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/noise.cpp

collision.o: utils/collision.cpp utils/collision.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/collision.cpp

model_loader.o: utils/model_loader.cpp utils/model_loader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/model_loader.cpp

image_loader.o: utils/image_loader.cpp utils/image_loader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/image_loader.cpp

shader_loader.o: utils/shader_loader.cpp utils/shader_loader.hpp
	$(CC) $(IFLAGS) $(CFLAGS) -c utils/shader_loader.cpp

utils.o: collision.o model_loader.o image_loader.o shader_loader.o noise.o
	$(CC) $(CFLAGS) -r collision.o model_loader.o image_loader.o shader_loader.o noise.o -o utils.o

# complete engine

engine.o: gl.o renderable.o scene_m.o utils.o
	$(CC) $(CFLAGS) -r gl.o renderable.o scene_m.o utils.o -o engine.o

clean:
	rm -f *.o
