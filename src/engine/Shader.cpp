#include "engine/Shader.hpp"
#include "util/GLDebug.hpp"

namespace lei3d 
{
	Shader::Shader()
	{
		// another clown emoji
	}

	Shader::Shader(const char* vertexShaderPath, const char* fragShaderPath)
	{
		// read from the files 
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open the files
			vShaderFile.open(vertexShaderPath);
			fShaderFile.open(fragShaderPath);
			std::stringstream vShaderStream;
			std::stringstream fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (const std::ifstream::failure& e)
		{
			if (vShaderFile.fail())
			{
				LEI_ERROR("ERROR - Failed to open vertex shader file: " + std::string(vertexShaderPath));
			}
			else if (fShaderFile.fail())
			{
				LEI_ERROR("ERROR - Failed to open fragment shader file: " + std::string(fragShaderPath));
			}
			else
			{
				// in my experience this outputs something like "basic_ios::clear: iostream error"
				// for something like a file not found. preferably the specific error would be output
				LEI_ERROR("ERROR - Shader File Not Successfully Read: " + std::string(e.what()));
			}
		}
		const char* vShaderCode = vertexCode.c_str(); // yeah I can work with c strings ( ͡° ͜ʖ ͡°)
		const char* fShaderCode = fragmentCode.c_str();

		// compile and configure shaders
		char infoLog[512];

		GLCall(unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER));
		GLCall(glShaderSource(vertexShaderID, 1, &vShaderCode, NULL));
		glCompileShader(vertexShaderID);

		int success;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
			LEI_ERROR("VERTEX SHADER COMPILATION FAILED\n\n" + std::string(infoLog));
		}

		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
			LEI_ERROR("FRAGMENT SHADER COMPILATION FAILED\n\n" + std::string(infoLog));
		}

		shaderProgramID = glCreateProgram(); // member variable
		glAttachShader(shaderProgramID, vertexShaderID);
		glAttachShader(shaderProgramID, fragmentShaderID);
		glLinkProgram(shaderProgramID);

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			LEI_ERROR("SHADER PROGRAM LINKING FAILED\n\n" + std::string(infoLog));
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	/** 
	 * Set OpenGL to use this shader. 
	 */
	void Shader::use() 
	{
		glUseProgram(shaderProgramID);
	}

	void Shader::setUniformMat4(glm::mat4& matrix, const char* matrixName) const
	{
		unsigned int projLoc = glGetUniformLocation(shaderProgramID, matrixName);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
    }

    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2f(glGetUniformLocation(shaderProgramID, name.c_str()), value.x, value.y);
    }
}