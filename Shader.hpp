#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int shaderProgramID;

	Shader(const char* vertexShaderPath, const char* fragShaderPath);
	// compile and link shader, then activate the shader
	void use();
};
#endif