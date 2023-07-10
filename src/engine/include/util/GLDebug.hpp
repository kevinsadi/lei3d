#pragma once

#include "include/util/Log.hpp"

#define GLCall(x) GLClearError();\
	x;\
	LEI_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);