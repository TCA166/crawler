
#pragma once

#include "engine/engine.hpp"
#include "objects/ship.hpp"
#include "objects/earth.hpp"
#include "objects/moon.hpp"

class game : public scene {
    private:
        double time_scale;
        double current_time, delta_time;
        bool mv_forward, mv_backward, mv_left, mv_right, mv_up, mv_down;
        double xpos, ypos;
        ship* our;
        earth* earth_obj;
        moon* moon_obj;
        shader* textured_shader;
        shader* planet_shader;
        light* sun;
    public:
        game();
        ~game();
        /*!
         @brief Initialize the scene
        */
        virtual void init();
        /*!
         @brief Main function of the scene
         @param target_camera The camera that the scene is being rendered with
        */
        virtual void main(camera* target_camera);
        /*!
         @brief Handle a scroll event
         @param xoffset The x offset of the scroll
         @param yoffset The y offset of the scroll
        */
        virtual void scroll_callback(double xoffset, double yoffset, camera* target_camera);
        /*!
         @brief Handle a key event
         @param key The key that was pressed
         @param scancode The scancode of the key
         @param action The action that was performed
         @param mods The mods that were pressed
        */
        virtual void key_callback(int key, int scancode, int action, int mods, camera* target_camera);
        /*!
         @brief Handle a mouse event
         @param xpos The x position of the mouse
         @param ypos The y position of the mouse
        */
        virtual void mouse_callback(double xpos, double ypos, camera* target_camera);
};

