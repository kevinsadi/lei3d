/*
 * This is a helper function used to define helper functions to create PCG terrain
 *
 * Author: Kevin Sadi
 */

#pragma once

#include <iostream>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "logging/Log.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace lei3d {
struct PlaneMesh {
	unsigned int planeVAO;
	unsigned int planeVBO;
	unsigned int planeEBO;
};

PlaneMesh* createPlaneMesh();
void createGroundPlane(float* groundPlaneVertices, int dim);
void createGroundPlaneTris(unsigned int* groundPlaneIndices, int dim);
std::vector<unsigned char> getElevationData();
}  // namespace lei3d