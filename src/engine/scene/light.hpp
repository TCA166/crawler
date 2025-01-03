
#pragma once

#include "../include.hpp"

#include <string>

/*!
 @brief a simple invisible directional light (like the sun)
 @note Infinite strength, infinite distance away
*/
class light {
  private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    unsigned int depthMapFBO;
    unsigned int depthMap;

  public:
    /*!
     @brief Constructs a light with a given position, color and attenuation
     parameters
     @param position the position of the light
     @param direction the direction of the light
     @param color the color of the light (rgb)
    */
    light(glm::vec3 position, glm::vec3 direction, glm::vec3 color);
    virtual ~light();
    /*!
     @brief Gets the direction of the light
     @return the direction of the light
    */
    const glm::vec3 &get_direction() const;
    /*!
     @brief Gets the color of the light
     @return the color of the light
    */
    const glm::vec3 &get_color() const;
    const glm::mat4 get_light_view() const;
    /*!
     @brief Sets the direction of the light
     @param direction the new direction of the light
    */
    void set_direction(glm::vec3 direction);
    /*!
     @brief Sets the color of the light
     @param color the new color of the light
    */
    void set_color(glm::vec3 color);
    /*!
     @brief Binds the depth map of the light
    */
    void bind_depth_map() const;
    void use_depth_map(int texture_unit) const;
};
