#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace lei3d 
{
	class Shader {
	private:
		unsigned int shaderProgramID;
	public:


		Shader();
		Shader(const char* vertexShaderPath, const char* fragShaderPath);

		
		// compile and link shader, then activate the shader
		void use();


        void setBool(const std::string& name, bool value) const;
		void setInt(const std::string &name, int value) const; // set string value in shader to an int
        void setFloat(const std::string& name, float value) const;

        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setUniformMat4(glm::mat4& matrix, const char* matrixName) const;
	};
}

#endif