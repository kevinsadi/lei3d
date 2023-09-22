#include "Shader.hpp"

#include "logging/GLDebug.hpp"

namespace lei3d
{

	Shader::Shader()
	{
		// another clown emoji
	}

	Shader::Shader(const char* vertexShaderPath, const char* fragShaderPath, const char* geomShaderPath)
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
		GLCall(glCompileShader(vertexShaderID));

		int success;
		GLCall(glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog));
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

		unsigned int geometryShaderID;
		if (geomShaderPath != nullptr)
		{
			std::string geometryCode;
			std::ifstream gShaderFile;
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				gShaderFile.open(geomShaderPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
			catch (const std::ifstream::failure& e)
			{
				if (gShaderFile.fail())
				{
					LEI_ERROR("ERROR - Failed to open geometry shader file: " + std::string(geomShaderPath));
				}
				else
				{
					LEI_ERROR("ERROR - Shader File Not Successfully Read: " + std::string(e.what()));
				}
			}
			const char* gShaderCode = geometryCode.c_str();

			GLCall(geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER));
			GLCall(glShaderSource(geometryShaderID, 1, &gShaderCode, NULL));
			GLCall(glCompileShader(geometryShaderID));

			success;
			GLCall(glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success));
			if (!success)
			{
				GLCall(glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog));
				LEI_ERROR("GEOMETRY SHADER COMPILATION FAILED\n\n" + std::string(infoLog));
			}
		}

		m_ShaderID = glCreateProgram(); // member variable
		glAttachShader(m_ShaderID, vertexShaderID);
		glAttachShader(m_ShaderID, fragmentShaderID);
		if (geomShaderPath != nullptr)
		{
			glAttachShader(m_ShaderID, geometryShaderID);
		}
		glLinkProgram(m_ShaderID);

		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
			LEI_ERROR("SHADER PROGRAM LINKING FAILED\n\n" + std::string(infoLog));
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		if (geomShaderPath != nullptr)
		{
			glDeleteShader(geometryShaderID);
		}
	}

	/**
	 * Set OpenGL to use this shader.
	 */
	void Shader::bind() const
	{
		GLCall(glUseProgram(m_ShaderID));
	}

	void Shader::unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		GLCall(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		GLCall(glUniform1i(getUniformLocation(name), static_cast<int>(value)));
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		GLCall(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		GLCall(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		GLCall(glUniform2f(getUniformLocation(name), value.x, value.y));
	}

	int Shader::getUniformLocation(const std::string& name) const
	{
		auto it = m_UniformLocationCache.find(name);
		if (it != m_UniformLocationCache.end())
		{
			return it->second;
		}

		GLCall(const int location = glGetUniformLocation(m_ShaderID, name.c_str()));
		if (location == -1)
		{
			LEI_ERROR("Uniform does not exist: " + name);
		}
		return location;
	}

} // namespace lei3d