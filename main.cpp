#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.hpp";

// no reference types here let's gooooo

// TODO: make header file for main when we refactor
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	// *** initialize window ***
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "kek3d", NULL, NULL);
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

	glViewport(0, 0, 800, 600);

	// resize the openGL context if a user changes the window size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// *** set up rendering pipeline ***
	Shader shader("Shaders/fragcolor.vert", "Shaders/fragcolor.frag");

	// note: each vertex's data is taken from the VBO currently specified as the array buffer
	float vertices[] = { 
		// positions
		0.7f, 0.7f, 0.0f,   1.0f, 0.0f, 0.0f,   // top right         0
		-0.7f, 0.7f, 0.0f,  0.0f, 1.0f, 0.0f,   // top left         1
		0.7f, -0.7f, 0.0f,  0.0f, 0.0f, 1.0f,    // bottom right     2
		-0.7f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f    // bottom left     3
	};
	unsigned int indices[] = {
		2, 0, 1, // first triangles
		3, 2, 1 // second triangles
	};

	unsigned int VAO; // vertex array object (containerize VBO and EBO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // all subsequent VBO and EBO operations are bound to this VAO

	unsigned int VBO; // vertex buffer object (store vertices)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO; // element buffer object (store indices)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	// position vert attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	// color vert attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// *** render loop ***
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.use();

		//float timeVal = glfwGetTime();
		//float greenVal = (sin(timeVal) + 1) / 2;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "squareColor");
		//glUniform4f(vertexColorLocation, 1.0f, greenVal, 1.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// swap chain and call callback functions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}