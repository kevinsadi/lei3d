/*
 * This is a helper function used to define helper functions to create PCG terrain
 * 
 * Author: Kevin Sadi
 */

#ifndef PCG_HELPERS
#define PCG_HELPERS

#include <vector>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
 
namespace lei3d {
    struct PlaneMesh
    {
        unsigned int planeVAO;
        unsigned int planeVBO;
        unsigned int planeEBO;
    };

    PlaneMesh* createPlaneMesh();
    void createGroundPlane(float* groundPlaneVertices, int dim);
    void createGroundPlaneTris(unsigned int* groundPlaneIndices, int dim);
    std::vector<unsigned char> getElevationData();
}

#endif