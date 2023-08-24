#pragma once

#include "logging/Log.hpp"

#define GLCall(x)   \
	GLClearError(); \
	x;              \
	GLLogCall(#x, __FILE__, __LINE__);

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);