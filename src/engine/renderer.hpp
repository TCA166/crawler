
#pragma once

#include "include.hpp"

#include "scene.hpp"
#include "camera.hpp"

#include <semaphore.h>

/*!
 @brief Renderer class to handle rendering of scenes.
 @details This class is used to render scenes and handle input.
*/
class renderer {
    private:
        GLFWwindow* window;
        scene* target_scene = NULL;
        camera* target_camera;
        int width, height;
        bool focused;
        sem_t* semaphore;
    public:
        /*!
         @brief Constructs a renderer with a given scene, width, height and name.
         @param width The width of the window
         @param height The height of the window
         @param name The name of the window
         @param semaphore The semaphore to use for synchronization
         @param target_camera The camera to use for rendering
         @note This will create a window and initialize GLEW
        */
        renderer(int width, int height, const char* name, sem_t* semaphore, camera* target_camera);
        /*!
         @brief Constructs a renderer with a given scene, width, height, name and parent window.
         @param width The width of the window
         @param height The height of the window
         @param name The name of the window
         @param semaphore The semaphore to use for synchronization
         @param target_camera The camera to use for rendering
         @param parent_window The parent window of the renderer
         @note This will create a window and initialize GLEW
        */
        renderer(int width, int height, const char* name, sem_t* semaphore, camera* target_camera, GLFWwindow* parent_window);
        ~renderer();
        /*!
         @brief Creates a new renderer based on this one, sharing the scene, semaphore and gl context
         @param name The name of the new window
         @return The new renderer
        */
        renderer clone(const char* name);
        /*!
         @brief Run the renderer
         @warning This function will block until the window is closed
        */
        void run();
        /*!
         @brief Change the scene to render
         @details This function will change the scene to render, and initialize the new scene
         @param new_scene The new scene to render
         @warning this must be called before run
        */
        void change_scene(scene* new_scene);
        /*!
         @brief Set the aspect ratio of the renderer
         @param width The width of the window
         @param height The height of the window
        */
        void resize(int width, int height);
        /*!
         @brief Focus the renderer
        */
        void focus();
        /*!
         @brief Unfocus the renderer
        */
        void unfocus();
        /*!
         @brief Handle a scroll event
         @param xoffset The x offset of the scroll
         @param yoffset The y offset of the scroll
        */
        void scroll_callback(double xoffset, double yoffset);
        /*!
         @brief Handle a key event
         @param key The key that was pressed
         @param scancode The scancode of the key
         @param action The action that was performed
         @param mods The mods that were pressed
        */
        void key_callback(int key, int scancode, int action, int mods);
        /*!
         @brief Handle a mouse button event
         @param button The button that was pressed
         @param action The action that was performed
         @param mods The mods that were pressed
        */
        void mouse_button_callback(int button, int action, int mods);
        /*!
         @brief Handle a mouse event
         @param xpos The x position of the mouse
         @param ypos The y position of the mouse
        */
        void mouse_callback(double xpos, double ypos);
        /*!
         @brief Handle a close event
        */
        void close_callback();
};
