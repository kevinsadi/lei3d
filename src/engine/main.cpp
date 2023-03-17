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

#include "include/engine/Shader.hpp"

// TODO: make header file for main when we refactor
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void createGroundPlane(float* groundPlaneVertices, int dim);
void createGroundPlaneTris(unsigned int* groundPlaneIndices, int dim);

// note: each vertex's data is taken from the VBO currently specified as the array buffer
float vertices[] = {  
	// face 1
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	// face 2
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	// face 3
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 // face 4
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 // face 5
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	// face 6
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

/*
float groundPlaneVertices[] = {
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
	0.0f, -0.5f, 6.0f, 0.0f, 1.0f, // 1
	6.0f, -0.5f, 6.0f, 1.0f, 1.0f, // 2
	6.0f, -0.5f, 0.0f, 1.0f, 0.0f  // 3
};
unsigned int groundPlaneIndices[] = {
	0, 2, 1,
	0, 3, 2
};
*/

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f,-3.0f),
	glm::vec3( 0.0f, 4.0f, 0.0f),
	glm::vec3(-5.0f,-0.0f, 1.0f),
	glm::vec3( 1.0f, 3.0f, 6.0f),
	glm::vec3(-0.0f, 7.0f,-0.0f),
	glm::vec3(-4.0f, 4.0f, 4.0f),
	glm::vec3( 1.0f, 3.0f, 5.0f)
};

// Get camera info using the Gram-Schmidt process
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 reverseCameraPointing = glm::normalize(cameraPos - cameraTarget);

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::cross(up, reverseCameraPointing); // right hand rule 

glm::vec3 cameraUp = glm::cross(reverseCameraPointing, cameraRight);

// ensure constant speed between frames
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera globals
bool firstMouse = true;
float lastX = 400;
float lastY = 300;
float pitch = 0.0;
float yaw = -90.f;
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

	GLFWwindow* window = glfwCreateWindow(1200,1000, "kek3d", NULL, NULL);
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

	glViewport(0, 0, 1200, 1000);

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

	unsigned int VAO; // vertex array object (containerize VBO and EBO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // all subsequent VBO and EBO operations are bound to this VAO

	unsigned int VBO; // vertex buffer object (store vertices)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position vert attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	// texture uv vert attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	// since I have to keep looking it up, the properties on this method is:
	// index, numcomponents per vert attribute, type, normalized, stride, offset from beginning


	/*     DEFINE PLANE      */
	const int dim = 128;

	float groundPlaneVertices[dim*dim*5];
	unsigned int groundPlaneIndices[(dim - 1) * (dim - 1) * 6];
	
	createGroundPlane(groundPlaneVertices, dim);
	createGroundPlaneTris(groundPlaneIndices, dim);

	for (int i = 0; i < dim * dim * 5; i++)
	{
		std::cout << groundPlaneVertices[i] << " ";
		if ((i+1) % 5 == 0) {
			std::cout << " " << std::endl;
		}
	}

	for (int i = 0; i < ((dim-1) * (dim-1) * 6); i++)
	{
		std::cout << groundPlaneIndices[i] << " ";
		if ((i + 1) % 3 == 0) {
			std::cout << " " << std::endl;
		}
	}


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

	// initialize projection matrix here because it rarely changes
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn off if UI
	glfwSetCursorPosCallback(window, mouse_callback);

	// ************************************** RENDER LOOP **********************************************
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// rendering
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw plane
		glBindVertexArray(planeVAO);
		glDrawElements(GL_TRIANGLES, ((dim-1)*(dim-1)*6), GL_UNSIGNED_INT, 0);

		// set VAO for the kirbies
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		// shaders 
		shader.use();
		shader.setUniformMat4(projection, "proj");
		
		// camera system
		const float radius = 25.0f;
		float cameraX = cos(glfwGetTime()) * radius;
		float cameraY = sin(glfwGetTime()) * radius;

		glm::mat4 view = glm::lookAt(glm::vec3(cameraX, 5.0f, cameraY), // param eye is misleading. It is the location of the camera
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 view;
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setUniformMat4(view, "view");

		// draw geometry
		for (glm::vec3 cubePosition : cubePositions)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, cubePosition);
			//model = glm::rotate(model, glm::radians(45.0f) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

			shader.setUniformMat4(model, "model");

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0); // unbind vertex array

		// swap chain and call callback functions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// ** glfw: called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ** glfw: called when there is mouse input
void mouse_callback(GLFWwindow* window, double xposInput, double yposInput)
{
	float xpos = static_cast<float>(xposInput);
	float ypos = static_cast<float>(yposInput);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // openGL inverted y
	lastX = xpos;
	lastY = ypos;

	float mouseSensitivity = 0.1f;
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// I am going to refactor this soon
	/*
	const float cameraSpeed = 7.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed;
	}
	*/
}


/*
 * Takes in two arrays by reference and populates them with the vertices to create a square ground plane made by vertices
 * 
 * 
 * Array should have size dim*dim*(3+2)           - explanation: dim*dim vertices. 3 vert points + 2 uv coords per vertex
 * 
 * @param groundPlaneVertices - 
 * @param groundPlaneUVs -
 * @param dim - this should be 128 for our purposes
 */
void createGroundPlane(float* groundPlaneVertices, int dim)
{
	// okay now lets try to make these ground plane verts and indices, but not manually
	for (int vertX = 0; vertX < dim; ++vertX)
	{
		for (int vertY = 0; vertY < dim; ++vertY)
		{
			int vertArrIndex = vertX * 5 + (vertY * 5 * dim); // start of this vertex in the array
			// vertex position
			groundPlaneVertices[vertArrIndex] = float(vertX);
			groundPlaneVertices[++vertArrIndex] = 0.0f;
			groundPlaneVertices[++vertArrIndex] = float(vertY);
			// uvs
			groundPlaneVertices[++vertArrIndex] = float(vertX) / dim;
			groundPlaneVertices[++vertArrIndex] = float(vertY) / dim;
		}
	}
}

/*
 * Takes in array by reference and populates it with the 
 *
 * Array should have size (dim-1)*(dim-1)*6.              - explanation = this gives us cubes * cubes. 2 tris per cube. 3 indices per tri.
 *
 * @param groundPlaneVertices -
 * @param groundPlaneUVs -
 * @param dim - this should be 128 for our purposes
 */
void createGroundPlaneTris(unsigned int* groundPlaneIndices, int dim)
{
	int colCheck = 0;
	int tNum = 0;

	// okay now lets try to make these ground plane verts and indices, but not manually
	for (int vi = 0; vi < (dim * dim) - dim; vi++)
	{
		colCheck += 1;
		if (colCheck % dim != 0) // do not include vertices on the last column
		{
			groundPlaneIndices[tNum]   = vi+1;     // right of current index
			groundPlaneIndices[tNum+1] = vi+dim+1; // right of directly above index
			groundPlaneIndices[tNum+2] = vi;       // current index

			groundPlaneIndices[tNum+3] = vi;       // current index
			groundPlaneIndices[tNum+4] = vi+dim+1; // right of directly above index
			groundPlaneIndices[tNum+5] = vi+dim;   // directly above index

			tNum += 6;
		}
	}
}