#include "GLDebug.hpp"

#include <glad/glad.h>

#include <sstream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::stringstream ss;
		std::string		  errorMessage;
		switch (error)
		{
			case GL_INVALID_ENUM:
				errorMessage = "INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errorMessage = "INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errorMessage = "INVALID_OPERATION";
				break;
			case GL_STACK_OVERFLOW:
				errorMessage = "STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				errorMessage = "STACK_UNDERFLOW";
				break;
			case GL_OUT_OF_MEMORY:
				errorMessage = "OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorMessage = "INVALID_FRAMEBUFFER_OPERATION";
				break;
		}
		ss << "[OpenGL Error] (" << errorMessage << ")" << function << " " << file << ":" << line;
		LEI_ERROR(ss.str());
		return false;
	}

	return true;
}