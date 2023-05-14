/*
 * This file is a part of Kek3D. 
 * 
 * Author: Kevin Sadi
 */


#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "include/engine/Shader.hpp"
#include "include/pcg/PCGHelpers.hpp"
#include "include/engine/FlyCamera.hpp"

using namespace kek3d;

// TODO: make header file for main when we refactor
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, FlyCamera* camera);

// ensure constant speed between frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float screenHeight = 1000;
float screenWidth = 1200;

// camera globals
bool firstMouse = true;
float lastX = screenWidth / 2.0f;  // RIGHT NOW, THIS WILL BREAK IF THE USER RESIZES THE GAME
float lastY = screenHeight / 2.0f;
float pitch = 0.0;
float yaw = -90.f;
float flySpeed = 3.0f;

/*
 * The main entrypoint for Kek3d (needs to be refactored)
 * 
 * Creates window, loads textures, loads vertices, has main render loop
 */
int main() {
	// *** initialize window ***
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(screenWidth,screenHeight, "kek3d", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	// resize the openGL context if a user changes the window size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// *** set up rendering pipeline ***
	// load shaders
	Shader shader("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

	// load textures
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set texture wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// load and generate texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./data/textures/rgb.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR: Texture loading failed" << std::endl;
	}
	glUniform1i(glGetUniformLocation(shader.shaderProgramID, "texture1"), 0);

	stbi_image_free(data); // no memory leaks here, no sir

	/*     DEFINE PLANE      */
	const int dim = 128;

	float groundPlaneVertices[dim*dim*5];
	unsigned int groundPlaneIndices[(dim - 1) * (dim - 1) * 6];
	
	createGroundPlane(groundPlaneVertices, dim);
	createGroundPlaneTris(groundPlaneIndices, dim);

	unsigned int planeVAO;
	unsigned int planeVBO;
	unsigned int planeEBO;

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);

	glBindVertexArray(planeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundPlaneVertices), groundPlaneVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundPlaneIndices), groundPlaneIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	FlyCamera* camera = new FlyCamera(window, -90.0f, 0.0f, 1.2f);
	glfwSetWindowUserPointer(window, camera);

	// initialize projection matrix here because it rarely changes
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn off if UI
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
	{
		if (FlyCamera* camera = static_cast<FlyCamera*>(glfwGetWindowUserPointer(window)))
		{
			camera->cameraMouseCallback(window, x, y);
		}
	});

	// ************************************** RENDER LOOP **********************************************
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window, camera);

		// rendering
		glClearColor(0.2, 0.8, 0.9, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw plane
		glBindVertexArray(planeVAO);
		glDrawElements(GL_TRIANGLES, ((dim-1)*(dim-1)*6), GL_UNSIGNED_INT, 0);

		// shaders 
		shader.use();
		shader.setUniformMat4(projection, "proj");
		
		// camera system
		glm::mat4 view = camera->getCameraView();
		shader.setUniformMat4(view, "view");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
		model = glm::translate(model, glm::vec3(-64, 0, -64));
		shader.setUniformMat4(model, "model");

		glBindVertexArray(0); // unbind vertex array

		// swap chain and call callback functions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &planeVAO);
	glDeleteBuffers(1, &planeEBO);
	glDeleteBuffers(1, &planeVBO);

	delete camera;

	// clear all of the previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

// ** glfw: called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, FlyCamera* camera)
{
	const float cameraSpeed = 7.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->handleForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->handleBack();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->handleLeft();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->handleRight();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		flySpeed = 5.0f; // TODO: Right now permanently just makes the camera fly speed to 5.0f lmfao, fix this so it's only temporary
	}
	// gracefully exit on escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
