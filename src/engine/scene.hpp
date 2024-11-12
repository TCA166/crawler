
#pragma once

#include "object.hpp"

#include <vector>

/*!
 @brief Scene class to handle rendering of objects.
 */
class scene {
    private:
        std::vector<object*> objects;
        std::vector<light*> lights;
        glm::vec3 ambient_light;
        glm::vec3 background_color;
        bool initialized = false;
        bool should_close = false;
    public:
        /*!
         @brief Constructs a scene
         @param ambient_light The ambient light of the scene
        */
        scene(glm::vec3 ambient_light, glm::vec3 background_color);
        ~scene();
        /*!
         @brief Add an object to the scene
         @param obj The object to add
        */
        /*!
         @brief Add an object to the scene
         @param obj The object to add
        */
        void add_object(object* obj);
        /*!
         @brief Add a light to the scene
         @param light The light to add
        */
        void add_light(light* light);
        /*!
         @brief Initialize the scene
        */
        virtual void init();
        /*!
         @brief Render the scene
         @param target_camera The camera to render the scene with
         @param aspect_ratio The aspect ratio of the window
        */
        void render(const camera* target_camera, float aspect_ratio);
        /*!
         @brief Main function of the scene
         @param target_camera The camera that the scene is being rendered with
        */
        virtual void main(camera* target_camera);
        /*!
         @brief Handle a scroll event
         @param xoffset The x offset of the scroll
         @param yoffset The y offset of the scroll
         @param target_camera The camera to scroll
        */
        virtual void scroll_callback(double xoffset, double yoffset, camera* target_camera);
        /*!
         @brief Handle a key event
         @param key The key that was pressed
         @param scancode The scancode of the key
         @param action The action that was performed
         @param mods The mods that were pressed
         @param target_camera The camera to handle the key event
        */
        virtual void key_callback(int key, int scancode, int action, int mods, camera* target_camera);
        /*!
         @brief Handle a mouse button event
         @param button The button that was pressed
         @param action The action that was performed
         @param mods The mods that were pressed
         @param target_camera The camera to handle the mouse button event
        */
        virtual void mouse_button_callback(int button, int action, int mods, camera* target_camera);
        /*!
         @brief Handle a mouse event
         @param xpos The x position of the mouse
         @param ypos The y position of the mouse
         @param target_camera The camera to handle the mouse event
        */
        virtual void mouse_callback(double xpos, double ypos, camera* target_camera);
        /*!
         @brief Handle a close event
        */
        void close_callback();
        /*!
         @brief Get the should close value
         @return The should close value
        */
        bool get_should_close() const;
};
