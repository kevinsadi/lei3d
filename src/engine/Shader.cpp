#include "include/engine/Shader.hpp"

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
		catch (std::ifstream::failure* e)
		{
			std::cout << "ERROR - Shader File Not Successfully Read" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str(); // yeah I can work with c strings ( ͡° ͜ʖ ͡°)
		const char* fShaderCode = fragmentCode.c_str();

		// compile and configure shaders
		char infoLog[512];

		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
		glCompileShader(vertexShaderID);

		int success;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
			std::cout << "VERTEX SHADER COMPILATION FAILED\n\n" << infoLog << std::endl;
		}

		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
			std::cout << "FRAGMENT SHADER COMPILATION FAILED\n\n" << infoLog << std::endl;
		}

		shaderProgramID = glCreateProgram(); // member variable
		glAttachShader(shaderProgramID, vertexShaderID);
		glAttachShader(shaderProgramID, fragmentShaderID);
		glLinkProgram(shaderProgramID);

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			std::cout << "SHADER PROGRAM LINKING FAILED\n\n" << infoLog << std::endl;
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

	void Shader::setUniformMat4(glm::mat4& matrix, const char* matrixName)
	{
		unsigned int projLoc = glGetUniformLocation(shaderProgramID, matrixName);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::setInt(const std::string &name, int value)
    { 
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
    }
}