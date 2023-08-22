#include "GLDebug.hpp"

#include <glad/glad.h>

#include <sstream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR)
		;
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::stringstream ss;
		ss << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line;
		LEI_ERROR(ss.str());
		return false;
	}

	return true;
}