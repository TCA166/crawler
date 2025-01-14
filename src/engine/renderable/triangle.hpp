
#pragma once

#include "object.hpp"

class triangle : public object {
  public:

    triangle(const shader *object_shader, double xpos, double ypos, double zpos);
    ~triangle();

    glm::vec3 update(const std::vector<triangle *> &boids, double deltaTime);
    bool is_active() const;     
    void set_active(bool value);
  private:
    bool active;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    glm::vec3 separation(const std::vector<triangle *> &boids);
    glm::vec3 alignment(const std::vector<triangle *> &boids);
    glm::vec3 cohesion(const std::vector<triangle *> &boids);

    static constexpr float maxSpeed = 8.0f;
    static constexpr float maxForce = 1.0f;
    static constexpr float separationDistance = 2.0f;
    static constexpr float alignmentDistance = 8.0f;
    static constexpr float cohesionDistance = 4.0f;
};
