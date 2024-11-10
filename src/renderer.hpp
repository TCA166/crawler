
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
        camera target_camera;
        float aspect_ratio;
        double xpos, ypos;
        bool focused;
        double current_time;
        double delta_time;
        bool mv_forward, mv_backward, mv_left, mv_right, mv_up, mv_down;
        sem_t* semaphore;
    public:
        /*!
         @brief Constructs a renderer with a given scene, width, height and name.
         @param width The width of the window
         @param height The height of the window
         @param name The name of the window
         @param semaphore The semaphore to use for synchronization
         @note This will create a window and initialize GLEW
        */
        renderer(int width, int height, const char* name, sem_t* semaphore);
        /*!
         @brief Constructs a renderer with a given scene, width, height, name and parent window.
         @param width The width of the window
         @param height The height of the window
         @param name The name of the window
         @param semaphore The semaphore to use for synchronization
         @param parent_window The parent window of the renderer
         @note This will create a window and initialize GLEW
        */
        renderer(int width, int height, const char* name, sem_t* semaphore, GLFWwindow* parent_window);
        ~renderer();
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
        /*!
         @brief Handle a mouse event
         @param xpos The x position of the mouse
         @param ypos The y position of the mouse
        */
        virtual void mouse_callback(double xpos, double ypos);
};
