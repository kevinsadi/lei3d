#ifndef TRANSFORM
#define TRANSFORM

#include <glm/glm.hpp>

// have to do this because I was getting recursive includes between physics and entity, 
// and I didn't want to forward declare
struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;       
    glm::vec3 scale{1, 1, 1};
};

#endif