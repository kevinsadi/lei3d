#pragma once

#include "glm/glm.hpp"

namespace lei3d {

class DirectionalLight {
public:
    DirectionalLight(glm::vec3 dir = glm::vec3{0, -1, 0}, glm::vec3 col = glm::vec3{1.f}, float intensity = 1.f) :
            direction(glm::normalize(dir)), color(col), intensity(intensity) {}

    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

} // lei3d
