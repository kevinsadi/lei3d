#pragma once

#include "logging/Log.hpp"

#if EDITOR
#define GLCall(x)   \
	GLClearError(); \
	x;              \
	GLLogCall(#x, __FILE__, __LINE__);
#else
#define GLCall(x) x;
#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);