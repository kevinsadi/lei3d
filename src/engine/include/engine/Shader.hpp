#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace kek3d 
{
	class Shader {
		
	public:
		unsigned int shaderProgramID;

		Shader(const char* vertexShaderPath, const char* fragShaderPath);
		
		// compile and link shader, then activate the shader
		void use();
		
		void setUniformMat4(glm::mat4& matrix, const char* matrixName);
	};
}

#endif