
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.hpp"
#include "camera.hpp"

/*!
 @brief Renderer class to handle rendering of scenes.
 @details This class is used to render scenes and handle input.
*/
class renderer {
    private:
        GLFWwindow* window;
        scene* target_scene;
        camera target_camera;
        float aspect_ratio;
        double xpos, ypos;
        bool focused;
        double current_time;
        double delta_time;
    public:
        /*!
         @brief Constructs a renderer with a given scene, width, height and name.
         @param target_scene The scene to render
         @param width The width of the window
         @param height The height of the window
         @param name The name of the window
         @note This will create a window and initialize GLEW
        */
        renderer(scene* target_scene, int width, int height, const char* name);
        ~renderer();
        /*!
         @brief Run the renderer
         @warning This function will block until the window is closed
        */
        void run();
        /*!
         @brief Change the scene to render
         @param new_scene The new scene to render
        */
        void change_scene(scene* new_scene);
        /*!
         @brief Set the aspect ratio of the renderer
         @param aspect_ratio The aspect ratio of the renderer
        */
        void set_aspect_ratio(float aspect_ratio);
        /*!
         @brief Handle a scroll event
         @param xoffset The x offset of the scroll
         @param yoffset The y offset of the scroll
        */
        virtual void scroll_callback(double xoffset, double yoffset);
        /*!
         @brief Handle a key event
         @param key The key that was pressed
         @param scancode The scancode of the key
         @param action The action that was performed
         @param mods The mods that were pressed
        */
        virtual void key_callback(int key, int scancode, int action, int mods);
        /*!
         @brief Handle a mouse button event
         @param button The button that was pressed
         @param action The action that was performed
         @param mods The mods that were pressed
        */
        virtual void mouse_button_callback(int button, int action, int mods);
};
