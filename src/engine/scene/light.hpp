
#pragma once

#include "../include.hpp"

class light {
  private:
    glm::vec3 position;
    glm::vec3 color;
    float constant;
    float linear;
    float quadratic;

  public:
    /*!
     @brief Constructs a light with a given position, color and attenuation
     parameters
     @param position the light's position
     @param color the color of the light (rgb)
     @param constant the constant attenuation of the light
     @param linear the linear attenuation of the light
     @param quadratic the quadratic attenuation of the light
    */
    light(glm::vec3 position, glm::vec3 color, float constant, float linear,
          float quadratic);
    virtual ~light();
    /*!
     @brief Gets the position of the light
     @return the position of the light
    */
    const glm::vec3 *get_position() const;
    /*!
     @brief Gets the color of the light
     @param the color of the light
    */
    const glm::vec3 *get_color() const;
    /*!
     @brief Gets the constant attenuation of the light
     @return the constant attenuation of the light
    */
    float get_constant() const;
    /*!
     @brief Gets the linear attenuation of the light
     @return the linear attenuation of the light
    */
    float get_linear() const;
    /*!
     @brief Gets the quadratic attenuation of the light
     @return the quadratic attenuation of the light
    */
    float get_quadratic() const;
};
