
#pragma once

#include "object.hpp"
#include <vector>

/*!
 @brief Scene class to handle rendering of objects.
 */
class scene {
    private:
        std::vector<const object*> objects;
    public:
        /*!
         @brief Constructs a scene
        */
        scene();
        ~scene();
        /*!
         @brief Add an object to the scene
         @param obj The object to add
        */
        void add_object(const object& obj);
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
};
