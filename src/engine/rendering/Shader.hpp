#pragma once

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

namespace lei3d 
{
	class Shader {
	private:
		unsigned int m_ShaderID;

		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		Shader();
		Shader(const char* vertexShaderPath, const char* fragShaderPath);
		
		// compile and link shader, then activate the shader
		void bind() const;
		void unbind() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string &name, int value) const; // set string value in shader to an int
        void setFloat(const std::string& name, float value) const;

        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		int getUniformLocation(const std::string& name) const;
	};
}