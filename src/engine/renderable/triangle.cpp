#include "triangle.hpp"
#include <glm/gtx/norm.hpp>

static const std::vector<float> triangle_data = {
    // positions          // normals           // texture coords // tangent           // bitangent
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  1.0f, 1.0f,        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
};

static const std::vector<unsigned int> triangle_indices = {
    0,1,2
};

triangle::triangle(const shader *object_shader, double xpos, double ypos,
                   double zpos)
    : object(object_shader, triangle_data, triangle_indices, xpos, ypos, zpos),
      position(glm::vec3(xpos, ypos, zpos)),
      velocity(glm::sphericalRand(
          1.0f)),
      acceleration(glm::vec3(0.0f)), 
        active(true) {}

triangle::~triangle() {}

glm::vec3 triangle::update(const std::vector<triangle *> &boids,
                           double deltaTime) {
    if (deltaTime == 0.0) {
        fprintf(stderr, "deltaTime is zero, no update performed.\n");
        return position;
    }

    deltaTime = deltaTime * 10000.0;

    glm::vec3 sep = separation(boids) * 4.0f; 
    glm::vec3 ali = alignment(boids) * 1.0f; 
    glm::vec3 coh = cohesion(boids) * 0.5f;   

    acceleration = sep + ali + coh;

    velocity += acceleration * deltaTime;

    if (glm::length(velocity) > maxSpeed) {
        velocity = glm::normalize(velocity) * maxSpeed;
    }

    position += velocity * deltaTime;

        // Boundary conditions
    if (position.x < -7.0f || position.x > 7.0f) {
        velocity.x = -velocity.x;
    }
    if (position.y < -7.0f || position.y > 7.0f) {
        velocity.y = -velocity.y;
    }
    if (position.z < -7.0f || position.z > 7.0f) {
        velocity.z = -velocity.z;
    }
    return position;
}

glm::vec3 triangle::separation(const std::vector<triangle *> &boids) {
    glm::vec3 steer(0.0f);
    int count = 0;
    for (const triangle *other : boids) {
        float distance = glm::length(position - other->position);
        if (distance > 0 && distance < separationDistance) {
            glm::vec3 diff = position - other->position;
            diff = glm::normalize(diff) / distance;
            steer += diff;
            count++;
        }
    }
    if (count > 0) {
        steer /= static_cast<float>(count);
    }
    if (glm::length(steer) > 0) {
        steer = glm::normalize(steer) * maxSpeed - velocity;
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce;
        }
    }
    return steer;
}

glm::vec3 triangle::alignment(const std::vector<triangle *> &boids) {
    glm::vec3 sum(0.0f);
    int count = 0;
    for (const triangle *other : boids) {
        float distance = glm::length(position - other->position);
        if (distance > 0 && distance < alignmentDistance) {
            sum += other->velocity;
            count++;
        }
    }
    if (count > 0) {
        sum /= static_cast<float>(count);
        sum = glm::normalize(sum) * maxSpeed;
        glm::vec3 steer = sum - velocity;
        if (glm::length(steer) > maxForce) {
            steer = glm::normalize(steer) * maxForce;
        }
        return steer;
    }
    return glm::vec3(0.0f);
}

glm::vec3 triangle::cohesion(const std::vector<triangle *> &boids) {
    glm::vec3 sum(0.0f);
    int count = 0;
    for (const triangle *other : boids) {
        float distance = glm::length(position - other->position);
        if (distance > 0 && distance < cohesionDistance) {
            sum += other->position;
            count++;
        }
    }
    if (count > 0) {
        sum /= static_cast<float>(count);
        return glm::normalize(sum - position) * maxSpeed - velocity;
    }
    return glm::vec3(0.0f);
}

bool triangle::is_active() const { return active; }

void triangle::set_active(bool value) { active = value; }